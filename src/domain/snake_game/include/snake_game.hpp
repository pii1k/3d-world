#pragma once

#include "camera_controller.hpp"
#include "game.hpp"
#include "input_controller.hpp"
#include "render_defs.hpp"

#include <memory>

namespace domain
{
struct GameState
{
    bool show_ui = true;
    glm::vec3 cube_pos{0.0f, 0.5f, 0.0f};
    float move_speed = 6.0f; // units/sec

    int window_width = 0;
    int window_height = 0;
};

class SnakeGame : public Game
{
public:
    std::unique_ptr<Game> createGame() override;
    void init() override;
    void update(float dt_s) override;
    void onEvent(const Event &e) override;
    void getScene(SceneData &out_scene) override;

private:
    GameState state_;
    std::unique_ptr<controller::CameraController> camera_controller_;
    std::unique_ptr<controller::InputController> input_controller_;
};
} // namespace domain