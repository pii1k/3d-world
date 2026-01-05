#pragma once

#include "camera.hpp"
#include "camera_system.hpp"
#include "input_controller.hpp"
#include "render_system.hpp"
#include "renderer.hpp"
#include "world.hpp"

#include <glm/glm.hpp>
#include <memory>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

struct Runtime
{
    float last_frame_time = 0.0f;
    float mouse_dx = 0.0f;
    float mouse_dy = 0.0f;
    float scroll_y = 0.0f;
};

struct Scene
{
    std::unique_ptr<World> world;

    entity_id ground_id{};
};

struct RenderContext
{
    GLFWwindow *window = nullptr;

    std::unique_ptr<Camera> camera;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<CameraSystem> camera_system;
    std::unique_ptr<RenderSystem> render_system;
    std::unique_ptr<InputController> input_controller;
};

class Engine
{
public:
    Engine();
    ~Engine() = default;

    void run();

    // GLFW callbacks
    void handleWindowResize(int width, int height);
    void handleMouseMove(double pos_x, double pos_y);
    void handleMouseScroll(double offset_x, double offset_y);
    void onMouseButton(int button, int action);

private:
    void init();
    void setupCallback();
    void loadAssets();

    void proccessInput(float delta_time);
    void update(float delta_time);
    void render();

    Runtime runtime_;
    Scene scene_;
    RenderContext render_ctx_;
};