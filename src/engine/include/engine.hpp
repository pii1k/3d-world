#pragma once

#include "game.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace engine
{
class Engine
{
public:
    Engine(domain::Game &game);
    ~Engine();

    bool init();
    void run();

private:
    struct MouseState
    {
        double last_x = 0.0;
        double last_y = 0.0;
        bool has_valid_last_pos = false;
    };

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow *window, double position_x, double position_y);
    static void keyboard_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
    void setGLFWCallbacks();

    void onFramebufferSize(int width, int height);
    void onScroll(double offset_y);
    void onMouseButton(int button, int action);
    void onCursorPosition(double position_x, double position_y);
    void onKey(int key, int action);

    void update(float dt_s);
    void render();

    GLFWwindow *window_ptr_ = nullptr;
    int framebuffer_w_ = 0;
    int framebuffer_h_ = 0;
    double last_time_s_ = 0.0;
    MouseState mouse_{};

    std::unique_ptr<graphics::Renderer> renderer_;
    std::unique_ptr<util::ResourceManager> resource_manager_;
    domain::Game &game_;
};
} // namespace engine