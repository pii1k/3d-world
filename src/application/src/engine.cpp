#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#include "component.hpp"
#include "engine.hpp"
#include "orbit_camera_system.hpp"
#include "prefabs.hpp"
#include "render_data.hpp"

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
    while (!render_ctx_.renderer->windowShouldClose())
    {
        const float current_frame_time = static_cast<float>(glfwGetTime());
        const float delta_time = current_frame_time - runtime_.last_frame_time;
        runtime_.last_frame_time = current_frame_time;

        this->proccessInput(delta_time);
        this->update(delta_time);
        this->render();
        render_ctx_.renderer->swapBuffers();
        render_ctx_.renderer->pollEvents();
    }
}

void Engine::handleWindowResize(int width, int height)
{
    const int clamped_width = std::max(width, 1);
    const int clamped_height = std::max(height, 1);
    glViewport(0, 0, clamped_width, clamped_height);
    if (render_ctx_.camera)
    {
        render_ctx_.camera->setAspectRatio(static_cast<float>(clamped_width) / static_cast<float>(clamped_height));
    }
}

void Engine::handleMouseMove(double xpos, double ypos)
{
    static double last_x = xpos;
    static double last_y = ypos;
    static bool first_mouse = true;

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    const float d_x = static_cast<float>(xpos - last_x);
    const float d_y = static_cast<float>(last_y - ypos);
    last_x = xpos;
    last_y = ypos;

    runtime_.mouse_dx += d_x;
    runtime_.mouse_dy += d_y;
}

void Engine::handleMouseScroll(double /* offset_x */, double offset_y)
{
    runtime_.scroll_y += static_cast<float>(offset_y);
}

void Engine::init()
{
    glfwSetErrorCallback(error_callback);

    // renderer
    render_ctx_.renderer = std::make_unique<Renderer>();
    if (!render_ctx_.renderer->init(kWidth, kHeight, kTitle))
    {
        throw std::runtime_error("Failed to initialize renderer");
    }

    render_ctx_.window = render_ctx_.renderer->getWindowPtr();

    // ecs
    scene_.world = std::make_unique<World>();
    render_ctx_.render_system = std::make_unique<RenderSystem>();

    scene_.ground_id = Prefabs::createGround(*scene_.world, static_cast<int>(MeshId::Plane), 50.0f);

    CameraConfig camera_config;
    camera_config.aspect_ratio = static_cast<float>(kWidth) / static_cast<float>(kHeight);
    render_ctx_.camera = std::make_unique<Camera>(glm::vec3{0.0f, 2.0f, 6.0f}, camera_config);

    // systems init
    render_ctx_.camera_system = std::make_unique<OrbitCameraSystem>();
}

void Engine::setupCallback()
{
    glfwSetWindowUserPointer(render_ctx_.window, this);
    glfwSetFramebufferSizeCallback(render_ctx_.window, frambuffer_size_callback);
    glfwSetCursorPosCallback(render_ctx_.window, mouse_callback);
    glfwSetScrollCallback(render_ctx_.window, scroll_callback);
    glfwSetInputMode(render_ctx_.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::loadAssets()
{
    // 월드의 다른 객체들(나무, 돌 등)을 생성합니다. 지금은 테스트용 큐브 하나만 추가.
    entity_id prop = scene_.world->newEntity();
    scene_.world->addComponent<TransformComponent>(prop, {{5.0f, 1.5f, -5.0f}, {}, {1.0f, 3.0f, 1.0f}});
    scene_.world->addComponent<RenderableComponent>(prop, RenderableComponent{static_cast<int>(MeshId::Cube), {0.3f, 0.6f, 1.0f}, false});
}

void Engine::proccessInput(float delta_time)
{
    if (glfwGetKey(render_ctx_.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(render_ctx_.window, true);
}

void Engine::update(float delta_time)
{
    if (!scene_.world || !render_ctx_.camera)
        return;

    render_ctx_.camera_system->update(runtime_.mouse_dx,
                                      runtime_.mouse_dy,
                                      runtime_.scroll_y,
                                      *render_ctx_.camera);
}

void Engine::render()
{
    RenderQueue render_queue;
    render_ctx_.render_system->buildRenderQueue(*scene_.world, render_queue);
    const glm::mat4 view = render_ctx_.camera->getViewMatrix();
    const glm::mat4 projection = render_ctx_.camera->getProjectionMatrix();
    render_ctx_.renderer->draw(render_queue, view, projection);
}
