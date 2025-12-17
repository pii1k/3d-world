#pragma once

#include <memory>

#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "camera_controller.hpp"
#include "registry.hpp"
#include "render_system.hpp"
#include "renderer.hpp"

class Engine
{
public:
    Engine();
    ~Engine() = default;

    void run();

    // intput handles: 콜백 함수들이 호출할거임
    void handleWindowResize(int width, int height);
    void handleMouseMove(double pos_x, double pos_y);
    void handleMouseScroll(double offset_x, double offset_y);

private:
    void init();
    void setupCallback();
    void loadAssets();

    void proccessInput(float delta_time);
    void update(float delta_time);
    void render();

    std::unique_ptr<Renderer> renderer_ptr_;
    std::unique_ptr<Registry> registry_ptr_;
    std::unique_ptr<RenderSystem> render_system_ptr_;
    std::unique_ptr<Camera> camera_ptr_;
    std::unique_ptr<CameraController> camera_controller_ptr_;

    GLFWwindow *window_ptr_ = nullptr;
    float last_frame_time = 0.0f;
};