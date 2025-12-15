#pragma once

#include <memory>

#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "registry.hpp"
#include "render_system.hpp"
#include "renderer.hpp"

class Engine
{
public:
    Engine();
    ~Engine() = default;

    void run();

    // intput handlers: 콜백 함수들이 호출할거임
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

    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Registry> registry_;
    std::unique_ptr<RenderSystem> render_system_;
    std::unique_ptr<Camera> camera_;

    GLFWwindow *window_ptr_ = nullptr;
    int framebuffer_width_ = 0;
    int framebuffer_height_ = 0;
    float last_frame_time = 0.0f;
    bool first_mouse_ = true;
    bool mouse_look_enabled_ = false;
    float last_x_ = 0.0f;
    float last_y_ = 0.0f;
};