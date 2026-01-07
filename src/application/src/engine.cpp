#include "input_controller.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include "camera_system.hpp"
#include "component.hpp"
#include "engine.hpp"
#include "prefabs.hpp"
#include "render_data.hpp"
#include <GLFW/glfw3.h>

namespace
{
constexpr float kWidth = 1280;
constexpr float kHeight = 720;
constexpr auto kTitle = "Autonomous Driving Simulation";

void frambuffer_size_callback(GLFWwindow *window_ptr, int width, int height)
{
    Engine *engine_ptr = static_cast<Engine *>(glfwGetWindowUserPointer(window_ptr));
    if (engine_ptr)
    {
        engine_ptr->handleWindowResize(width, height);
    }
}

void mouse_callback(GLFWwindow *window_ptr, double pos_x, double pos_y)
{
    Engine *engine_ptr = static_cast<Engine *>(glfwGetWindowUserPointer(window_ptr));
    if (engine_ptr)
    {
        engine_ptr->handleMouseMove(pos_x, pos_y);
    }
}

void mouse_button_callback(GLFWwindow *window_ptr, int button, int action, int /*mods*/)
{
    Engine *engine_ptr = static_cast<Engine *>(glfwGetWindowUserPointer(window_ptr));
    if (engine_ptr)
        engine_ptr->handleMouseButton(button, action);
}

void scroll_callback(GLFWwindow *window_ptr, double offset_x, double offset_y)
{
    Engine *engine_ptr = static_cast<Engine *>(glfwGetWindowUserPointer(window_ptr));
    if (engine_ptr)
    {
        engine_ptr->handleMouseScroll(offset_x, offset_y);
    }
}

void error_callback(int error_code, const char *description)
{
    std::cerr << "[GLFW] error " << error_code << ": " << (description ? description : "unknown") << "\n";
}

} // namespace

Engine::Engine()
{
    this->init();
    this->setupCallback();
    this->loadAssets();
}

void Engine::run()
{
    while (!render_ctx_.view.renderer->windowShouldClose())
    {
        const float current_frame_time = static_cast<float>(glfwGetTime());
        const float delta_time = current_frame_time - runtime_.last_frame_time;
        runtime_.last_frame_time = current_frame_time;

        this->proccessInput(delta_time);
        this->update(delta_time);
        this->render();
        render_ctx_.view.renderer->swapBuffers();
        render_ctx_.view.renderer->pollEvents();
    }
}

void Engine::handleWindowResize(int width, int height)
{
    const int clamped_width = std::max(width, 1);
    const int clamped_height = std::max(height, 1);
    glViewport(0, 0, clamped_width, clamped_height);
    if (render_ctx_.view.camera)
    {
        render_ctx_.view.camera->setAspectRatio(static_cast<float>(clamped_width) / static_cast<float>(clamped_height));
    }
}

void Engine::handleMouseMove(double xpos, double ypos)
{
    if (render_ctx_.view.input_controller)
    {
        render_ctx_.view.input_controller->onMouseMove(xpos, ypos);
    }
}

void Engine::handleMouseScroll(double /* offset_x */, double offset_y)
{
    if (render_ctx_.view.input_controller)
    {
        render_ctx_.view.input_controller->onScroll(offset_y);
    }
}

void Engine::handleMouseButton(int button, int action)
{
    if (action != GLFW_PRESS || button != GLFW_MOUSE_BUTTON_LEFT)
        return;
    if (!render_ctx_.view.input_controller || !render_ctx_.view.camera || !render_ctx_.view.window || !scene_.world)
        return;

    double cursor_x = 0.0;
    double cursor_y = 0.0;
    glfwGetCursorPos(render_ctx_.view.window, &cursor_x, &cursor_y);

    int window_width = 0;
    int window_height = 0;
    glfwGetWindowSize(render_ctx_.view.window, &window_width, &window_height);

    const glm::mat4 view = render_ctx_.view.camera->getViewMatrix();
    const glm::mat4 proj = render_ctx_.view.camera->getProjectionMatrix();

    if (scene_.selected_entity)
    {
        scene_.world->removeComponent<SelectedComponent>(*scene_.selected_entity);
        scene_.selected_entity.reset();
    }

    bool hit = false;
    scene_.world->forEachComponent<SelectableComponent>(
        [&](entity_id entity, SelectableComponent &)
        {
            if (hit)
                return;

            auto transform_opt = scene_.world->getComponent<TransformComponent>(entity);
            if (!transform_opt)
                return;

            const TransformComponent &transform = transform_opt->get();
            glm::vec3 half_extents = transform.scale * 0.5f;
            glm::vec3 center_offset = glm::vec3(0.0f);

            if (auto pick_bounds_opt = scene_.world->getComponent<PickBoundsComponent>(entity))
            {
                const PickBoundsComponent &pb = pick_bounds_opt->get();
                half_extents = pb.half_extents;
                center_offset = pb.center_offset;
            }

            const glm::vec3 bounds_center = transform.position + center_offset;
            if (render_ctx_.view.input_controller->onMouseClick(cursor_x, cursor_y,
                                                                window_width, window_height,
                                                                view, proj, bounds_center,
                                                                half_extents))
            {
                scene_.world->addComponent<SelectedComponent>(entity, SelectedComponent{});
                scene_.selected_entity = entity;
                std::cerr << "[input] selected entity " << entity << std::endl;
                hit = true;
            }
        });
}

void Engine::init()
{
    glfwSetErrorCallback(error_callback);

    // renderer
    render_ctx_.view.renderer = std::make_unique<Renderer>();
    if (!render_ctx_.view.renderer->init(kWidth, kHeight, kTitle))
    {
        throw std::runtime_error("Failed to initialize renderer");
    }

    render_ctx_.view.window = render_ctx_.view.renderer->getWindowPtr();

    scene_.world = std::make_unique<World>();
    render_ctx_.systems.render_system = std::make_unique<RenderSystem>();

    scene_.ground_id = Prefabs::createGround(*scene_.world, static_cast<int>(MeshId::Plane), 50.0f);

    CameraConfig camera_config;
    camera_config.aspect_ratio = static_cast<float>(kWidth) / static_cast<float>(kHeight);
    render_ctx_.view.camera = std::make_unique<Camera>(glm::vec3{0.0f, 2.0f, 6.0f}, camera_config);

    render_ctx_.systems.camera_system = std::make_unique<QuarterViewCameraSystem>();
    render_ctx_.view.input_controller = std::make_unique<InputController>();
}

void Engine::setupCallback()
{
    glfwSetWindowUserPointer(render_ctx_.view.window, this);
    glfwSetFramebufferSizeCallback(render_ctx_.view.window, frambuffer_size_callback);
    glfwSetCursorPosCallback(render_ctx_.view.window, mouse_callback);
    glfwSetMouseButtonCallback(render_ctx_.view.window, mouse_button_callback);
    glfwSetScrollCallback(render_ctx_.view.window, scroll_callback);
    glfwSetInputMode(render_ctx_.view.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Engine::loadAssets()
{
    // TODO(jyan): MVP는 일단 임시로 이렇게.. 나중에 수정 필요
    auto spawn_vehicle = [&](const glm::vec3 &position,
                             const glm::vec3 &color,
                             const glm::vec3 &scale) -> entity_id
    {
        entity_id body = scene_.world->newEntity();
        scene_.world->addComponent<TransformComponent>(body, TransformComponent{position, {}, scale});
        scene_.world->addComponent<RenderableComponent>(body, RenderableComponent{static_cast<int>(MeshId::Cube), color, false});
        scene_.world->addComponent<SelectableComponent>(body, SelectableComponent{});
        const glm::vec3 body_half_extents = scale * 0.5f;
        scene_.world->addComponent<PickBoundsComponent>(body, PickBoundsComponent{body_half_extents, {}});

        // roof
        const glm::vec3 roof_scale = glm::vec3(scale.x * 0.6f, scale.y * 0.5f, scale.z * 0.6f);
        const glm::vec3 roof_pos = position + glm::vec3(0.0f, (scale.y + roof_scale.y) * 0.5f, 0.0f);
        entity_id roof = scene_.world->newEntity();
        scene_.world->addComponent<TransformComponent>(roof, TransformComponent{roof_pos, {}, roof_scale});
        scene_.world->addComponent<RenderableComponent>(roof, RenderableComponent{static_cast<int>(MeshId::Cube), color, false});

        return body;
    };

    auto spawn_traffic_light = [&](const glm::vec3 &position,
                                   const glm::vec3 &color,
                                   const glm::vec3 &scale) -> entity_id
    {
        entity_id cube = scene_.world->newEntity();
        const glm::vec3 grounded_pos = position + glm::vec3(0.0f, scale.y * 0.5f, 0.0f);
        scene_.world->addComponent<TransformComponent>(cube, TransformComponent{grounded_pos, {}, scale});
        scene_.world->addComponent<RenderableComponent>(cube, RenderableComponent{static_cast<int>(MeshId::Cube), color, false});
        scene_.world->addComponent<SelectableComponent>(cube, SelectableComponent{});
        const glm::vec3 half_extents = scale * 0.5f;
        scene_.world->addComponent<PickBoundsComponent>(cube, PickBoundsComponent{half_extents, {}});
        return cube;
    };

    spawn_vehicle({-4.0f, 1.0f, -5.0f},
                  {0.7f, 0.3f, 0.3f},
                  {1.6f, 1.0f, 3.2f});

    spawn_traffic_light({4.0f, 0.0f, -3.0f},
                        {0.3f, 0.6f, 1.0f},
                        {0.35f, 3.0f, 0.35f});
}

void Engine::proccessInput(float delta_time)
{
    if (glfwGetKey(render_ctx_.view.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(render_ctx_.view.window, true);
}

void Engine::update(float delta_time)
{
    if (!scene_.world || !render_ctx_.view.camera || !render_ctx_.systems.camera_system)
        return;

    if (render_ctx_.view.input_controller)
    {
        render_ctx_.view.input_controller->cameraUpdate(delta_time,
                                                        *render_ctx_.systems.camera_system,
                                                        *render_ctx_.view.camera);
    }
}

void Engine::render()
{
    RenderQueue render_queue;
    render_ctx_.systems.render_system->buildRenderQueue(*scene_.world, render_queue);
    const glm::mat4 view = render_ctx_.view.camera->getViewMatrix();
    const glm::mat4 projection = render_ctx_.view.camera->getProjectionMatrix();
    render_ctx_.view.renderer->draw(render_queue, view, projection);
}
