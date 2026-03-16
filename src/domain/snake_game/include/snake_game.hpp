#pragma once

#include "camera_controller.hpp"
#include "game.hpp"
#include "render_defs.hpp"
#include "snake_input.hpp"
#include "snake_logic.hpp"
#include "snake_scene_builder.hpp"

#include <memory>

namespace domain
{
struct GameState
{
    bool show_ui = false;
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
    SnakeLogic logic_;
    SnakeInput input_;
    SnakeSceneBuilder scene_builder_;
};
} // namespace domain
