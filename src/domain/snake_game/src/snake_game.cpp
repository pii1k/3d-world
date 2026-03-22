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
    camera_rig_.init(state_.window_width, state_.window_height);
}

void SnakeGame::update(float dt_s)
{
    camera_rig_.update();
    logic_.update(dt_s);
}

void SnakeGame::onEvent(const Event &e)
{
    std::visit([this](auto &&arg)
               {
                   using T = std::decay_t<decltype(arg)>;

                   if constexpr (std::is_same_v<T, ScrollEvent>)
                   {
                       camera_rig_.onScroll(arg.offset_y);
                   }
                   else if constexpr (std::is_same_v<T, MouseButtonEvent>)
                   {
                       // do nothing
                   }
                   else if constexpr (std::is_same_v<T, CursorMoveEvent>)
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
                       camera_rig_.onResize(arg.width, arg.height);
                   }
                   else
                   {
                       std::cerr << "Undefined event type." << std::endl;
                   } },
               e);
}

void SnakeGame::setScene(SceneData &out_scene)
{
    out_scene.view = camera_rig_.getCamera().view();
    out_scene.proj = camera_rig_.getCamera().proj();
    out_scene.show_ui = state_.show_ui || logic_.state().paused || logic_.state().game_over;
    out_scene.objects.clear();

    scene_builder_.buildScene(logic_, out_scene);
}

} // namespace domain

std::unique_ptr<domain::Game> create_game()
{
    return std::make_unique<domain::SnakeGame>();
}
