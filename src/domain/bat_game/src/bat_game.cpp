#include "bat_game.hpp"
#include "event.hpp"

#include <iostream>
#include <type_traits>
#include <variant>

namespace domain
{
std::unique_ptr<Game> BatGame::createGame()
{
    return std::make_unique<BatGame>();
}

void BatGame::init()
{
    logic_.init();
    camera_rig_.init(state_.window_width, state_.window_height);
}

void BatGame::update(float dt_s)
{
    const InputState input = input_.currentState();
    logic_.update(dt_s, input);
    camera_rig_.update(dt_s, logic_.state());
}

void BatGame::onEvent(const Event &e)
{
    std::visit([this](auto &&arg)
               {
                   using T = std::decay_t<decltype(arg)>;

                   if constexpr (std::is_same_v<T, ScrollEvent>)
                   {
                       // do nothing for now
                   }
                   else if constexpr (std::is_same_v<T, MouseButtonEvent>)
                   {
                       // do nothing for now
                   }
                   else if constexpr (std::is_same_v<T, CursorMoveEvent>)
                   {
                       input_.onCursorMove(arg.delta_x, arg.delta_y);
                   }
                   else if constexpr (std::is_same_v<T, KeyEvent>)
                   {
                       input_.onKeyEvent(arg.key, arg.action);
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

void BatGame::setScene(SceneData &out_scene)
{
    out_scene.view = camera_rig_.getCamera().view();
    out_scene.proj = camera_rig_.getCamera().proj();
    out_scene.show_ui = state_.show_ui;
    out_scene.objects.clear();

    scene_builder_.buildScene(logic_, out_scene);
}
} // namespace domain

std::unique_ptr<domain::Game> create_game()
{
    return std::make_unique<domain::BatGame>();
}
