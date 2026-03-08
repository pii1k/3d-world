#include "snake_game.hpp"
#include "colors.hpp"
#include "event.hpp"

#include <glm/gtc/matrix_transform.hpp>
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

    input_controller_ = std::make_unique<controller::InputController>(state_.cube_pos,
                                                                      state_.show_ui,
                                                                      state_.move_speed);
}

void SnakeGame::update(float dt_s)
{
    if (input_controller_)
        input_controller_->update(dt_s);
    if (camera_controller_)
        camera_controller_->update();
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
                       if (input_controller_)
                           input_controller_->onMouseClick(arg.x, arg.y,
                                                           state_.window_width, state_.window_height,
                                                           camera_controller_->getCamera().view(),
                                                           camera_controller_->getCamera().proj());
                   }
                   else if constexpr (std::is_same_v<T, KeyEvent>)
                   {
                       if (input_controller_)
                           input_controller_->onKey(arg.key, arg.action);
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
    out_scene.show_ui = state_.show_ui;
    out_scene.objects.clear();

    // Ground
    {
        constexpr int kGridW = 24;
        constexpr int kGridH = 18;
        constexpr float kGroundThickness = 1.0f;
        const float ground_width = static_cast<float>(kGridW);
        const float ground_length = static_cast<float>(kGridH) * 2.0f;

        const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -kGroundThickness * 0.5f, 0.0f)) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(ground_width, kGroundThickness, ground_length));

        out_scene.objects.push_back({domain::MeshType::Cube, // Using cube for ground for now
                                     model,
                                     graphics::colors::kGrassGreenAlt});
    }

    // Snake cube
    {
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), state_.cube_pos);
        out_scene.objects.push_back({domain::MeshType::Cube,
                                     model,
                                     graphics::colors::kWaterBlue});
    }
}

} // namespace domain

std::unique_ptr<domain::Game> create_game()
{
    return std::make_unique<domain::SnakeGame>();
}