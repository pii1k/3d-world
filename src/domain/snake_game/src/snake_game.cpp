#include "snake_game.hpp"
#include "event.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <type_traits>
#include <variant>

namespace domain
{
std::unique_ptr<Game> SnakeGame::createGame()
{
    return std::make_unique<SnakeGame>();
}

void SnakeGame::init()
{
    CameraConfig cam_cfg{};
    cam_cfg.position = glm::vec3(0.0f, 2.0f, 12.0f);
    cam_cfg.target = glm::vec3(0.0f, 0.0f, 0.0f);
    cam_cfg.up = glm::vec3(0.0f, 1.0f, 0.0f);
    cam_cfg.fov_deg = 45.0f;
    cam_cfg.near_plane = 0.1f;
    cam_cfg.far_plane = 200.0f;
    camera_controller_ = std::make_unique<controller::CameraController>(cam_cfg);
    camera_controller_->update();
}

void SnakeGame::update(float dt_s)
{
    if (camera_controller_)
        camera_controller_->update();
    logic_.update(dt_s);
}

void SnakeGame::onEvent(const Event &e)
{
    std::visit([this](auto &&arg)
               {
                   using T = std::decay_t<decltype(arg)>;

                   if constexpr (std::is_same_v<T, ScrollEvent>)
                   {
                       if (camera_controller_)
                           camera_controller_->onScroll(arg.offset_y);
                   }
                   else if constexpr (std::is_same_v<T, MouseButtonEvent>)
                   {
                       // do nothing
                   }
                   else if constexpr (std::is_same_v<T, KeyEvent>)
                   {
                       if (arg.action == GLFW_PRESS)
                           input_.onKeyPressed(arg.key, logic_, state_.show_ui);
                   }
                   else if constexpr (std::is_same_v<T, ResizeEvent>)
                   {
                       state_.window_width = arg.width;
                       state_.window_height = arg.height;
                       if (camera_controller_)
                           camera_controller_->onResize(arg.width, arg.height);
                   }
                   else
                   {
                       std::cerr << "Undefined event type." << std::endl;
                   } },
               e);
}

void SnakeGame::getScene(SceneData &out_scene)
{
    if (!camera_controller_)
        return;

    out_scene.view = camera_controller_->getCamera().view();
    out_scene.proj = camera_controller_->getCamera().proj();
    out_scene.show_ui = state_.show_ui || logic_.state().paused || logic_.state().game_over;
    out_scene.objects.clear();

    scene_builder_.buildScene(logic_, out_scene);
}

} // namespace domain

std::unique_ptr<domain::Game> create_game()
{
    return std::make_unique<domain::SnakeGame>();
}