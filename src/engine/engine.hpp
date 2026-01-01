#pragma once

#include "glad/gl.h"
#include "input/input_controller.hpp"
#include "renderer/shader.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>

class Engine
{
public:
    Engine() = default;
    ~Engine();

    bool init();
    void run();

private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void keyboard_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
    void setGLFWCallbacks();

    void onFramebufferSize(int width, int height);
    void onScroll(double yoffset);
    void onMouseButton(int button, int action);
    void onKey(int key, int action);

    void update(float dt_s);
    void render();

    struct AppState
    {
        explicit AppState(Camera camera)
            : cam_ctrl(std::move(camera)) {}

        OrbitCamera cam_ctrl;
        bool show_ui = false;
        glm::vec3 cube_pos{0.0f, 0.5f, 0.0f};
        float move_speed = 6.0f; // units/sec
    };

    GLFWwindow *window_ptr_ = nullptr;
    int framebuffer_w_ = 0;
    int framebuffer_h_ = 0;
    double last_time_s_ = 0.0;

    std::unique_ptr<AppState> state_;
    std::unique_ptr<InputController> input_controller_;
    std::unique_ptr<Shader> shader_;

    // TODO(jyan): 아래는 renderer, mesh 클래스 나오면 없앨거임
    GLint u_mvp_loc_ = -1;
    GLint u_color_loc_ = -1;
    GLuint cube_vao_ = 0;
    GLuint cube_vbo_ = 0;
    GLuint ui_vao_ = 0;
    GLuint ui_vbo_ = 0;
};