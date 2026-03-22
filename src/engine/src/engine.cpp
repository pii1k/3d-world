#include "engine.hpp"
#include "event.hpp"

#include <algorithm>
#include <iostream>

namespace
{
using namespace std::string_literals;

constexpr int kWindowW = 960;
constexpr int kWindowH = 720;

engine::Engine *get_engine(GLFWwindow *window)
{
    return static_cast<engine::Engine *>(glfwGetWindowUserPointer(window));
}
} // namespace

namespace engine
{
Engine::Engine(domain::Game &game)
    : game_(game) {}

Engine::~Engine()
{
    if (window_ptr_)
    {
        glfwDestroyWindow(window_ptr_);
        window_ptr_ = nullptr;
    }
    glfwTerminate();
}

bool Engine::init()
{
    // init OpenGL
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
    glfwSetInputMode(window_ptr_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    // init resource manager
    resource_manager_ = std::make_unique<util::ResourceManager>();
    resource_manager_->setRootPath(RESOURCE_ROOT_PATH);

    // init renderer
    renderer_ = std::make_unique<graphics::Renderer>(*resource_manager_);
    if (!renderer_->init())
    {
        std::cerr << "Failed to initialize Renderer\n";
        return false;
    }

    last_time_s_ = glfwGetTime();
    game_.init();
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
        game_.update(dt_s);
        this->render();
        glfwSwapBuffers(window_ptr_);
    }
}

void Engine::update(float dt_s)
{
    game_.update(dt_s);
}

void Engine::render()
{
    if (!renderer_)
        return;

    glViewport(0, 0, framebuffer_w_, framebuffer_h_);
    glClearColor(0.06f, 0.07f, 0.09f, 3.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    domain::SceneData scene;
    game_.setScene(scene);

    renderer_->draw(scene);
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

void Engine::cursor_position_callback(GLFWwindow *window, double position_x, double position_y)
{
    if (auto *eng = get_engine(window))
        eng->onCursorPosition(position_x, position_y);
}

void Engine::setGLFWCallbacks()
{
    glfwSetFramebufferSizeCallback(window_ptr_, Engine::framebuffer_size_callback);
    glfwSetKeyCallback(window_ptr_, Engine::keyboard_callback);
    glfwSetMouseButtonCallback(window_ptr_, Engine::mouse_button_callback);
    glfwSetCursorPosCallback(window_ptr_, Engine::cursor_position_callback);
    glfwSetScrollCallback(window_ptr_, Engine::scroll_callback);
}

void Engine::onFramebufferSize(int width, int height)
{
    framebuffer_w_ = width;
    framebuffer_h_ = height;
    game_.onEvent(domain::ResizeEvent{framebuffer_w_, framebuffer_h_});
}

void Engine::onScroll(double offset_y)
{
    game_.onEvent(domain::ScrollEvent{0.0, offset_y});
}

void Engine::onMouseButton(int button, int action)
{
    if (!window_ptr_)
        return;
    if (!(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS))
        return;

    double cursor_x = 0.0;
    double cursor_y = 0.0;
    glfwGetCursorPos(window_ptr_, &cursor_x, &cursor_y);

    game_.onEvent(domain::MouseButtonEvent{button, action, 0, cursor_x, cursor_y});
}

void Engine::onCursorPosition(double position_x, double position_y)
{
    if (!window_ptr_)
        return;

    double delta_x = 0.0;
    double delta_y = 0.0;
    if (mouse_.has_valid_last_pos)
    {
        delta_x = position_x - mouse_.last_x;
        delta_y = position_y - mouse_.last_y;
    }
    else
    {
        mouse_.has_valid_last_pos = true;
    }

    mouse_.last_x = position_x;
    mouse_.last_y = position_y;

    game_.onEvent(domain::CursorMoveEvent{position_x, position_y, delta_x, delta_y});
}

void Engine::onKey(int key, int action)
{
    if (!window_ptr_)
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

    game_.onEvent(domain::KeyEvent{key, 0, action, 0});
}
} // namespace engine
