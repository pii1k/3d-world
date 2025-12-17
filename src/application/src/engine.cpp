#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "GLFW/glfw3.h"
#include "component.hpp"
#include "engine.hpp"
#include "registry.hpp"
#include "third_person_camera_controller.hpp"

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
    while (!renderer_ptr_->windowShouldClose())
    {
        float current_frame_time = static_cast<float>(glfwGetTime());
        float delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        this->proccessInput(delta_time);
        this->update(delta_time);
        this->render();
        renderer_ptr_->swapBuffers();
        renderer_ptr_->pollEvents();
    }
}

void Engine::handleWindowResize(int width, int height)
{
    // 너비/높이가 0인 상태로 viewport와 투영행렬을 만들면 NaN이 생겨서 장면이 사라질 수 있음
    const int clamped_width = std::max(width, 1);
    const int clamped_height = std::max(height, 1);
    glViewport(0, 0, clamped_width, clamped_height);
    camera_ptr_->setAspectRatio(static_cast<float>(clamped_width) / static_cast<float>(clamped_height));
}

void Engine::handleMouseMove(double xpos, double ypos)
{
    // 마우스 입력 처리는 이제 컨트롤러의 책임입니다.
    // (Engine은 더 이상 last_x, last_y를 알 필요가 없습니다. 컨트롤러가 내부적으로 처리)
    static float last_x = xpos;
    static float last_y = ypos;
    static bool first_mouse = true;

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    if (camera_controller_ptr_)
    {
        camera_controller_ptr_->processMouseMovement(xoffset, yoffset);
    }
}

void Engine::handleMouseScroll(double offset_x, double offset_y)
{
    (void)offset_x;
    if (camera_controller_ptr_)
    {
        camera_controller_ptr_->processMouseScroll(static_cast<float>(offset_y));
    }
}

void Engine::init()
{
    glfwSetErrorCallback(error_callback);

    // renderer
    renderer_ptr_ = std::make_unique<Renderer>();
    if (!renderer_ptr_->init(kWidth, kHeight, kTitle))
    {
        throw std::runtime_error("Failed to initialize renderer");
    }

    window_ptr_ = renderer_ptr_->getWindowPtr();

    // ecs
    registry_ptr_ = std::make_unique<Registry>();
    render_system_ptr_ = std::make_unique<RenderSystem>();

    // TODO(jyan): player entity -> 나중에 scene이나 world 클래스로 대체
    Entity player_entity = registry_ptr_->createEntity();
    TransformComponent player_transform = {{0.0f, 0.5f, 0.0f}};
    registry_ptr_->addTransform(player_entity, player_transform);
    registry_ptr_->addRenderable(player_entity, RenderableComponent{0}); // 플레이어를 임시로 큐브로 표시

    // 4. 카메라와 컨트롤러 생성 및 주입
    CameraConfig camera_config;
    camera_config.aspect_ratio = 1280.0f / 720.0f;

    ThirdPersonControllerConfig controller_config;
    controller_config.distance_to_target = 7.0f;

    camera_ptr_ = std::make_unique<Camera>(player_transform.position, camera_config);
    camera_controller_ptr_ =
        std::make_unique<ThirdPersonCameraController>(*camera_ptr_, *registry_ptr_, player_entity, controller_config);
}

void Engine::setupCallback()
{
    glfwSetWindowUserPointer(window_ptr_, this);
    glfwSetFramebufferSizeCallback(window_ptr_, frambuffer_size_callback);
    glfwSetCursorPosCallback(window_ptr_, mouse_callback);
    glfwSetScrollCallback(window_ptr_, scroll_callback);
    glfwSetInputMode(window_ptr_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Engine::loadAssets()
{
    // 월드의 다른 객체들(나무, 돌 등)을 생성합니다.
    registry_ptr_->addTransform(registry_ptr_->createEntity(), {{5.0f, 0.0f, -5.0f}, {}, {1.0f, 3.0f, 1.0f}});
    registry_ptr_->addRenderable(registry_ptr_->createEntity(), RenderableComponent{0});
}

void Engine::proccessInput(float delta_time)
{
    if (glfwGetKey(window_ptr_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_ptr_, true);
}

void Engine::update(float delta_time)
{
    if (camera_controller_ptr_)
    {
        camera_controller_ptr_->update(delta_time);
    }
    // 나중에 물리 시스템, ai 시스템 등의 업데이트 여기에 정의
}

void Engine::render()
{
    RenderQueue render_queue;                                 // 프레임별 렌더 큐
    render_system_ptr_->update(*registry_ptr_, render_queue); // ECS 데이터를 draw command로 변환
    const glm::mat4 view = camera_ptr_->getViewMatrix();
    const glm::mat4 projection = camera_ptr_->getProjectionMatrix();
    renderer_ptr_->draw(render_queue, view, projection); // 큐를 실제로 렌더링
}