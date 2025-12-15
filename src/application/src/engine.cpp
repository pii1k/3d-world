#include <iostream>
#include <memory>
#include <stdexcept>

#include "GLFW/glfw3.h"
#include "component.hpp"
#include "engine.hpp"
#include "registry.hpp"
#include <glm/glm.hpp>

namespace
{
constexpr float kWidth = 1280;
constexpr float kHeight = 720;
constexpr auto kTitle = "Autonomous Driving Simulation";
const auto kCameraInitalPos = glm::vec3(0.0f, 0.5f, 3.0f);

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
    while (!renderer_->windowShouldClose())
    {
        float current_frame_time = static_cast<float>(glfwGetTime());
        float delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        this->proccessInput(delta_time);
        this->update(delta_time);
        this->render();
    }
}

void Engine::handleWindowResize(int width, int height)
{
    // 너비/높이가 0인 상태로 viewport와 투영행렬을 만들면 NaN이 생겨서 장면이 사라질 수 있음
    const int clamped_width = std::max(width, 1);
    const int clamped_height = std::max(height, 1);
    framebuffer_width_ = clamped_width;
    framebuffer_height_ = clamped_height;
    glViewport(0, 0, clamped_width, clamped_height);
    camera_->setAspectRatio(static_cast<float>(clamped_width) / static_cast<float>(clamped_height));
    std::clog << "[engine] framebuffer " << framebuffer_width_ << "x" << framebuffer_height_ << std::endl;
}

void Engine::handleMouseMove(double pos_x, double pos_y)
{
    // 마우스 오른쪽 버튼을 누르는 동안에만 카메라 회전 허용
    if (glfwGetMouseButton(window_ptr_, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
    {
        if (mouse_look_enabled_)
        {
            // 우클릭 해제 시 커서 복구
            glfwSetInputMode(window_ptr_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        mouse_look_enabled_ = false;
        first_mouse_ = true;
        return;
    }

    if (first_mouse_)
    {
        last_x_ = static_cast<float>(pos_x);
        last_y_ = static_cast<float>(pos_y);
        first_mouse_ = false;
        return;
    }

    float offset_x = static_cast<float>(pos_x) - last_x_;
    float offset_y = last_y_ - static_cast<float>(pos_y);
    last_x_ = static_cast<float>(pos_x);
    last_y_ = static_cast<float>(pos_y);

    camera_->processMouseMovement(offset_x, offset_y);
}

void Engine::handleMouseScroll(double offset_x, double offset_y)
{
    (void)offset_x;
    camera_->processMouseScroll(static_cast<float>(offset_y));
}

void Engine::init()
{
    glfwSetErrorCallback(error_callback);

    renderer_ = std::make_unique<Renderer>();
    if (!renderer_->init(kWidth, kHeight, kTitle))
    {
        throw std::runtime_error("Failed to initialize renderer");
    }

    window_ptr_ = renderer_->getWindowPtr();
    registry_ = std::make_unique<Registry>();
    render_system_ = std::make_unique<RenderSystem>();
    camera_ = std::make_unique<Camera>(kCameraInitalPos);
    last_x_ = kWidth / 2;
    last_y_ = kHeight / 2;
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
    Entity cube = registry_->createEntity();                                 // 첫 번째 큐브 엔티티 생성
    registry_->addTransform(cube, TransformComponent{{-1.5F, 0.0F, -5.0F}}); // 위치 설정
    registry_->addRenderable(cube, RenderableComponent{0});                  // 큐브 모델 ID 부여

    Entity cube2 = registry_->createEntity();                                // 두 번째 큐브 생성
    registry_->addTransform(cube2, TransformComponent{{1.5F, 0.5F, -6.0F}}); // 위치 설정
    registry_->addRenderable(cube2, RenderableComponent{0});                 // 동일한 큐브 메시 사용
}

void Engine::proccessInput(float delta_time)
{
    if (glfwGetKey(window_ptr_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_ptr_, true);
    if (glfwGetKey(window_ptr_, GLFW_KEY_W) == GLFW_PRESS)
        camera_->processKeyborad('W', delta_time);
    if (glfwGetKey(window_ptr_, GLFW_KEY_S) == GLFW_PRESS)
        camera_->processKeyborad('S', delta_time);
    if (glfwGetKey(window_ptr_, GLFW_KEY_A) == GLFW_PRESS)
        camera_->processKeyborad('A', delta_time);
    if (glfwGetKey(window_ptr_, GLFW_KEY_D) == GLFW_PRESS)
        camera_->processKeyborad('D', delta_time);
}

void Engine::update(float /* delta_time */)
{
    // 나중에 물리 시스템, ai 시스템 등의 업데이트 여기에 정의
}

void Engine::render()
{
    RenderQueue render_queue;                         // 프레임별 렌더 큐
    render_system_->update(*registry_, render_queue); // ECS 데이터를 draw command로 변환
    const glm::mat4 view = camera_->getViewMatrix();
    const glm::mat4 projection = camera_->getProjectionMatrix();
    renderer_->draw(render_queue, view, projection); // 큐를 실제로 렌더링
    renderer_->swapBuffers();                        // 새 프레임을 화면에 표시
    renderer_->pollEvents();                         // X 이벤트 처리 및 종료 여부 업데이트
}
