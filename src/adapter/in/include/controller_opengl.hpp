#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

namespace ads::adapter
{
struct Vec3
{
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};
};

class ControllerOpengl
{
public:
    ControllerOpengl(unsigned int width, unsigned int height);

    void pollEvents();
    bool isActive() const { return window_active_; }

    sf::RenderWindow &window() { return window_; }
    unsigned int width() const { return width_; }
    unsigned int height() const { return height_; }

    float cameraDistance() const { return camera_distance_; }
    float cameraYawDeg() const { return camera_yaw_deg_; }
    float cameraPitchDeg() const { return camera_pitch_deg_; }
    const Vec3 &cameraTargetOffset() const { return camera_target_offset_; }
    void nudgeCameraTargetOffset(float dx, float dy, float dz);

private:
    void handleEvent(const sf::Event &event);

    unsigned int width_;
    unsigned int height_;
    sf::RenderWindow window_;
    bool window_active_{true};
    bool orbiting_{false};
    bool panning_{false};
    sf::Vector2i last_mouse_position_{};
    Vec3 camera_target_offset_{};
    float camera_distance_{35.0F};
    float camera_yaw_deg_{-110.0F};
    float camera_pitch_deg_{35.0F};
    float camera_pan_speed_{20.0F};
    float keyboard_pan_speed_{20.0F};
};
} // namespace ads::adapter
