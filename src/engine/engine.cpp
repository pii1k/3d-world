#include "engine/engine.hpp"

#include "renderer/colors.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
constexpr int kGridW = 24;
constexpr int kGridH = 18;
constexpr int kWindowW = 960;
constexpr int kWindowH = 720;
constexpr glm::mat4 kIMat = glm::mat4(1.0f);
const glm::vec3 kUIPanelColor{0.10f, 0.12f, 0.16f};

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

Engine *get_engine(GLFWwindow *window)
{
    return static_cast<Engine *>(glfwGetWindowUserPointer(window));
}
} // namespace

Engine::~Engine()
{
    if (window_ptr_)
        glfwMakeContextCurrent(window_ptr_);

    if (ui_vbo_)
        glDeleteBuffers(1, &ui_vbo_);
    if (ui_vao_)
        glDeleteVertexArrays(1, &ui_vao_);

    if (cube_vbo_)
        glDeleteBuffers(1, &cube_vbo_);
    if (cube_vao_)
        glDeleteVertexArrays(1, &cube_vao_);

    shader_.reset();
    input_controller_.reset();
    state_.reset();

    if (window_ptr_)
    {
        glfwDestroyWindow(window_ptr_);
        window_ptr_ = nullptr;
    }
    glfwTerminate();
}

bool Engine::init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window_ptr_ = glfwCreateWindow(kWindowW, kWindowH, "3d Snake (GLFW/OpenGL)", nullptr, nullptr);
    if (!window_ptr_)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_ptr_);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window_ptr_, this);
    this->setGLFWCallbacks();
    glfwGetFramebufferSize(window_ptr_, &framebuffer_w_, &framebuffer_h_);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glEnable(GL_MULTISAMPLE);

    CameraConfig cam_cfg{};
    cam_cfg.position = glm::vec3(0.0f, 2.0f, 12.0f);
    cam_cfg.target = glm::vec3(0.0f, 0.0f, 0.0f);
    cam_cfg.up = glm::vec3(0.0f, 1.0f, 0.0f);
    cam_cfg.fov_deg = 45.0f;
    cam_cfg.near_plane = 0.1f;
    cam_cfg.far_plane = 200.0f;

    state_ = std::make_unique<AppState>(Camera(cam_cfg));
    state_->cam_ctrl.update();
    input_controller_ = std::make_unique<InputController>(state_->cam_ctrl,
                                                          state_->cube_pos,
                                                          state_->show_ui,
                                                          state_->move_speed);

    const std::string shader_dir = std::string(ASSETS_DIR) + "/shader/";
    shader_ = std::make_unique<Shader>(shader_dir + "snake.vs", shader_dir + "snake.fs");
    shader_->use();

    u_mvp_loc_ = glGetUniformLocation(shader_->getId(), "uMVP");
    u_color_loc_ = glGetUniformLocation(shader_->getId(), "uColor");

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &cube_vao_);
    glGenBuffers(1, &cube_vbo_);
    glBindVertexArray(cube_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    glGenVertexArrays(1, &ui_vao_);
    glGenBuffers(1, &ui_vbo_);
    glBindVertexArray(ui_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kUIPanelVertices), kUIPanelVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    last_time_s_ = glfwGetTime();
    return true;
}

void Engine::run()
{
    if (!window_ptr_)
        return;

    while (!glfwWindowShouldClose(window_ptr_))
    {
        const double now_s = glfwGetTime();
        const float dt_s = static_cast<float>(std::max(0.0, now_s - last_time_s_));
        last_time_s_ = now_s;

        glfwPollEvents();
        this->update(dt_s);
        this->render();
        glfwSwapBuffers(window_ptr_);
    }
}

void Engine::update(float dt_s)
{
    if (input_controller_)
        input_controller_->update(dt_s);
}

void Engine::render()
{
    if (!shader_ || !state_)
        return;

    glViewport(0, 0, framebuffer_w_, framebuffer_h_);
    glClearColor(0.06f, 0.07f, 0.09f, 3.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_->use();

    auto &camera = state_->cam_ctrl.getCamera();
    const glm::mat4 view = camera.view();
    const glm::mat4 proj = camera.proj(framebuffer_w_, framebuffer_h_);

    // ground
    {
        constexpr float kGroundThickness = 1.0f;
        const float ground_width = static_cast<float>(kGridW);
        const float ground_length = static_cast<float>(kGridH) * 2.0f;

        const glm::mat4 model = glm::translate(kIMat, glm::vec3(0.0f, -kGroundThickness * 0.5f, 0.0f)) *
                                glm::scale(kIMat, glm::vec3(ground_width, kGroundThickness, ground_length));
        const glm::mat4 mvp = proj * view * model;

        glUniform3f(u_color_loc_, colors::kGrassGreenAlt.r, colors::kGrassGreenAlt.g, colors::kGrassGreenAlt.b);
        glUniformMatrix4fv(u_mvp_loc_, 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(cube_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    // snake cube
    {
        const glm::mat4 model = glm::translate(kIMat, state_->cube_pos);
        const glm::mat4 mvp = proj * view * model;

        glUniform3f(u_color_loc_, colors::kWaterBlue.r, colors::kWaterBlue.g, colors::kWaterBlue.b);
        glUniformMatrix4fv(u_mvp_loc_, 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(cube_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    // ui
    if (state_->show_ui)
    {
        glDisable(GL_DEPTH_TEST);
        glUniform3f(u_color_loc_, kUIPanelColor.r, kUIPanelColor.g, kUIPanelColor.b);
        glUniformMatrix4fv(u_mvp_loc_, 1, GL_FALSE, glm::value_ptr(kIMat));
        glBindVertexArray(ui_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
}

void Engine::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    if (auto *eng = get_engine(window))
        eng->onFramebufferSize(width, height);
}

void Engine::scroll_callback(GLFWwindow *window, double /*xoffset*/, double yoffset)
{
    if (auto *eng = get_engine(window))
        eng->onScroll(yoffset);
}

void Engine::mouse_button_callback(GLFWwindow *window, int button, int action, int /*mods*/)
{
    if (auto *eng = get_engine(window))
        eng->onMouseButton(button, action);
}

void Engine::keyboard_callback(GLFWwindow *window, int key, int /*scan_code*/, int action, int /*mods*/)
{
    if (auto *eng = get_engine(window))
        eng->onKey(key, action);
}

void Engine::setGLFWCallbacks()
{
    glfwSetFramebufferSizeCallback(window_ptr_, Engine::framebuffer_size_callback);
    glfwSetKeyCallback(window_ptr_, Engine::keyboard_callback);
    glfwSetMouseButtonCallback(window_ptr_, Engine::mouse_button_callback);
    glfwSetScrollCallback(window_ptr_, Engine::scroll_callback);
}

void Engine::onFramebufferSize(int width, int height)
{
    framebuffer_w_ = width;
    framebuffer_h_ = height;
}

void Engine::onScroll(double yoffset)
{
    if (input_controller_)
        input_controller_->onScroll(yoffset);
}

void Engine::onMouseButton(int button, int action)
{
    if (!input_controller_ || !state_ || !window_ptr_)
        return;
    if (!(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS))
        return;

    double cursor_x = 0.0;
    double cursor_y = 0.0;
    glfwGetCursorPos(window_ptr_, &cursor_x, &cursor_y);

    auto &camera = state_->cam_ctrl.getCamera();
    const glm::mat4 view = camera.view();
    const glm::mat4 proj = camera.proj(framebuffer_w_, framebuffer_h_);
    input_controller_->onMouseClick(cursor_x, cursor_y, framebuffer_w_, framebuffer_h_, view, proj);
}

void Engine::onKey(int key, int action)
{
    if (!state_ || !window_ptr_)
        return;

    const bool pressed = (action == GLFW_PRESS);
    const bool released = (action == GLFW_RELEASE);
    if (!pressed && !released)
        return;

    if (pressed && key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window_ptr_, GLFW_TRUE);
        return;
    }
    if (input_controller_)
        input_controller_->onKey(key, action);
}