#include "colors.hpp"
#include "controller.hpp"
#include "glad/gl.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "shader.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

namespace
{
constexpr int kGridW = 24;
constexpr int kGridH = 18;
constexpr int kWindowW = 960;
constexpr int kWindowH = 720;
constexpr glm::mat4 kIMat = glm::mat4(1.0f);
const glm::vec3 kUIPanelColor{0.10f, 0.12f, 0.16f};

int g_framebuffer_w = kWindowW;
int g_framebuffer_h = kWindowH;
void framebuffer_size_callback(GLFWwindow * /* window */, int width, int height)
{
    g_framebuffer_w = width;
    g_framebuffer_h = height;
    glViewport(0, 0, width, height);
}

const float kCubeVertices[] = {
    // front (z = +0.5)
    -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
    // back (z = -0.5)
    -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
    // left (x = -0.5)
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
    // right (x = +0.5)
    0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
    // top (y = +0.5)
    -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
    // bottom (y = -0.5)
    -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f};

const float kUIPanelVertices[] = {
    // triangle 1
    -0.95f, 0.95f, 0.0f, -0.55f, 0.95f, 0.0f, -0.55f, 0.65f, 0.0f,
    // triangle 2
    -0.95f, 0.95f, 0.0f, -0.55f, 0.65f, 0.0f, -0.95f, 0.65f, 0.0f};

struct AppState
{
    controller::OrbitCamera cam_ctrl;
    bool show_ui = false;
};

GLFWwindow *init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(kWindowW, kWindowH, "3d Snake (GLFW/OpenGL)", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwGetFramebufferSize(window, &g_framebuffer_w, &g_framebuffer_h);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return nullptr;
    }

    glEnable(GL_MULTISAMPLE);

    return window;
}

void scroll_callback(GLFWwindow *window, double /* xoffset */, double yoffset)
{
    auto *state = static_cast<AppState *>(glfwGetWindowUserPointer(window));
    if (!state)
        return;

    state->cam_ctrl.onScroll(yoffset);
}

void keyboard_callback(GLFWwindow *window, int key, int /*scan_code*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_U)
    {
        auto *state = static_cast<AppState *>(glfwGetWindowUserPointer(window));
        if (!state)
            return;

        state->show_ui = !state->show_ui;
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        glfwTerminate();
    }
}

} // namespace

int main()
{
    GLFWwindow *window = init();
    if (!window)
        return -1;

    CameraConfig cam_cfg{};
    cam_cfg.position = glm::vec3(0.0f, 2.0f, 12.0f);
    cam_cfg.target = glm::vec3(0.0f, 0.0f, 0.0f);
    cam_cfg.up = glm::vec3(0.0f, 1.0f, 0.0f);
    cam_cfg.fov_deg = 45.0f;
    cam_cfg.near_plane = 0.1f;
    cam_cfg.far_plane = 200.0f;

    AppState app_state{controller::OrbitCamera{Camera(cam_cfg)}, false};
    app_state.cam_ctrl.update(); // initialize
    glfwSetWindowUserPointer(window, &app_state);

    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetScrollCallback(window, scroll_callback);

    const std::string shader_dir = std::string(ASSETS_DIR) + "/shader/";
    Shader shader(shader_dir + "snake.vs", shader_dir + "snake.fs");
    shader.use();

    const GLint u_mvp_loc = glGetUniformLocation(shader.getId(), "uMVP");
    const GLint u_color_loc = glGetUniformLocation(shader.getId(), "uColor");

    glEnable(GL_DEPTH_TEST);
    glUniform3f(u_color_loc, colors::kGrassGreenAlt.r, colors::kGrassGreenAlt.g, colors::kGrassGreenAlt.b);

    // set cube
    GLuint plane_vao, plane_vbo = 0;
    glGenVertexArrays(1, &plane_vao);
    glGenBuffers(1, &plane_vbo);
    glBindVertexArray(plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    // set ui
    GLuint ui_vao, ui_vbo = 0;
    glGenVertexArrays(1, &ui_vao);
    glGenBuffers(1, &ui_vbo);
    glBindVertexArray(ui_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kUIPanelVertices), kUIPanelVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    // loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glViewport(0, 0, g_framebuffer_w, g_framebuffer_h);
        glClearColor(0.06f, 0.07f, 0.09f, 3.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        constexpr float kGroundThickness = 1.0f;
        const float ground_width = static_cast<float>(kGridW);
        const float ground_length = static_cast<float>(kGridH) * 2.0f;

        const glm::mat4 model = glm::translate(kIMat, glm::vec3(0.0f, -kGroundThickness * 0.5f, 0.0f)) *
                                glm::scale(kIMat, glm::vec3(ground_width, kGroundThickness, ground_length));

        auto &camera = app_state.cam_ctrl.getCamera();
        const glm::mat4 view = camera.view();
        const glm::mat4 proj = camera.proj(g_framebuffer_w, g_framebuffer_h);
        const glm::mat4 mvp = proj * view * model;
        glUniform3f(u_color_loc, colors::kGrassGreenAlt.r, colors::kGrassGreenAlt.g, colors::kGrassGreenAlt.b);
        glUniformMatrix4fv(u_mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(plane_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        if (app_state.show_ui)
        {
            glDisable(GL_DEPTH_TEST);
            glUniform3f(u_color_loc, kUIPanelColor.r, kUIPanelColor.g, kUIPanelColor.b);
            glUniformMatrix4fv(u_mvp_loc, 1, GL_FALSE, glm::value_ptr(kIMat));
            glBindVertexArray(ui_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            glEnable(GL_DEPTH_TEST);
        }

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &ui_vbo);
    glDeleteVertexArrays(1, &ui_vao);

    glDeleteBuffers(1, &plane_vbo);
    glDeleteVertexArrays(1, &plane_vao);

    glfwTerminate();
    return 0;
}
