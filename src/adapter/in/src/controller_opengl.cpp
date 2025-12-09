#include "controller_opengl.hpp"

#include <algorithm>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace
{
constexpr float kMinCameraDistance = 5.0F;
constexpr float kMaxCameraDistance = 150.0F;
}

namespace ads::adapter
{
ControllerOpengl::ControllerOpengl(unsigned int width, unsigned int height)
    : width_(width),
      height_(height),
      window_(sf::VideoMode(width_, height_), "Autonomous Driving Simulator", sf::Style::Default)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.antialiasingLevel = 4;
    window_.setActive(true);
    window_.setVerticalSyncEnabled(true);
    last_mouse_position_ = sf::Mouse::getPosition(window_);
}

void ControllerOpengl::pollEvents()
{
    sf::Event event{};
    while (window_.pollEvent(event))
    {
        handleEvent(event);
    }
}

void ControllerOpengl::handleEvent(const sf::Event &event)
{
    switch (event.type)
    {
    case sf::Event::Closed:
        window_active_ = false;
        window_.close();
        break;
    case sf::Event::Resized:
        width_ = event.size.width;
        height_ = event.size.height;
        break;
    case sf::Event::MouseWheelScrolled:
        camera_distance_ -= event.mouseWheelScroll.delta * 2.5F;
        camera_distance_ = std::clamp(camera_distance_, kMinCameraDistance, kMaxCameraDistance);
        break;
    case sf::Event::MouseButtonPressed:
        last_mouse_position_ = {event.mouseButton.x, event.mouseButton.y};
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            orbiting_ = true;
        }
        if (event.mouseButton.button == sf::Mouse::Middle)
        {
            panning_ = true;
        }
        break;
    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            orbiting_ = false;
        }
        if (event.mouseButton.button == sf::Mouse::Middle)
        {
            panning_ = false;
        }
        break;
    case sf::Event::MouseMoved:
    {
        const sf::Vector2i current(event.mouseMove.x, event.mouseMove.y);
        const sf::Vector2i delta = current - last_mouse_position_;
        last_mouse_position_ = current;
        if (orbiting_)
        {
            camera_yaw_deg_ -= static_cast<float>(delta.x) * 0.25F;
            camera_pitch_deg_ -= static_cast<float>(delta.y) * 0.25F;
            camera_pitch_deg_ = std::clamp(camera_pitch_deg_, -5.0F, 85.0F);
        }
        else if (panning_)
        {
            const float pan_scale = camera_distance_ * 0.005F;
            camera_target_offset_.x -= static_cast<float>(delta.x) * pan_scale;
            camera_target_offset_.z += static_cast<float>(delta.y) * pan_scale;
        }
        break;
    }
    default:
        break;
    }
}

void ControllerOpengl::nudgeCameraTargetOffset(float dx, float dy, float dz)
{
    camera_target_offset_.x += dx;
    camera_target_offset_.y += dy;
    camera_target_offset_.z += dz;
}
} // namespace ads::adapter
