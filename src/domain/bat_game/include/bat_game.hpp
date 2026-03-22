#pragma once

#include "bat_camera_rig.hpp"
#include "bat_input.hpp"
#include "bat_logic.hpp"
#include "bat_scene_builder.hpp"
#include "game.hpp"

#include <memory>

namespace domain
{
struct BatGameState
{
    bool show_ui = false;
    int window_width = 0;
    int window_height = 0;
};

class BatGame : public Game
{
public:
    std::unique_ptr<Game> createGame() override;
    void init() override;
    void update(float dt_s) override;
    void onEvent(const Event &e) override;
    void setScene(SceneData &out_scene) override;

private:
    BatGameState state_{};
    BatInput input_{};
    BatLogic logic_{};
    BatSceneBuilder scene_builder_{};
    BatCameraRig camera_rig_{};
};
} // namespace domain