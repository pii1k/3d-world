#pragma once

#include "camera.hpp"
#include "camera_system.hpp"
#include "input_controller.hpp"
#include "render_system.hpp"
#include "renderer.hpp"
#include "world.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

struct Runtime
{
    float last_frame_time = 0.0f;
};

struct Scene
{
    std::unique_ptr<World> world;
    entity_id ground_id{};
    std::vector<entity_id> selectable_entities;
};

struct ViewContext
{
    std::unique_ptr<Renderer> renderer;
    GLFWwindow *window = nullptr;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<InputController> input_controller;
};

struct SystemsContext
{
    std::unique_ptr<CameraSystem> camera_system;
    std::unique_ptr<RenderSystem> render_system;
};

struct RenderContext
{
    ViewContext view;
    SystemsContext systems;
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
    void handleMouseButton(int button, int action);

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