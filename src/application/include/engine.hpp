#pragma once

#include <memory>

#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "player_movement_system.hpp"
#include "render_system.hpp"
#include "renderer.hpp"
#include "third_person_camera_system.hpp"
#include "world.hpp"

struct Runtime
{
    float last_frame_time = 0.0f;
};

struct Scene
{
    std::unique_ptr<World> world;

    entity_id player_id{};
    entity_id ground_id{};
    entity_id camera_id{};
};

struct RenderContext
{
    std::unique_ptr<Renderer> renderer;
    GLFWwindow *window = nullptr;

    std::unique_ptr<Camera> camera;
    std::unique_ptr<RenderSystem> render_system;
};

struct Systems
{
    std::unique_ptr<PlayerMovementSystem> player_movement;
    std::unique_ptr<ThirdPersonCameraSystem> third_person_camera;
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
    Systems systems_;
};