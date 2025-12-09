#pragma once

#include <array>
#include <memory>
#include <span>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "pedestrian.hpp"
#include "render_port.hpp"
#include "vehicle.hpp"

namespace ads::adapter
{
struct Vec3
{
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};
};

class GlRenderAdapter : public application::RenderPort
{
public:
    GlRenderAdapter(unsigned int width, unsigned int height);

    void render(const domain::World &world) override;
    bool isActive() const override { return window_active_; }

private:
    void ensureWindow();
    void processEvents();
    void setupProjection();
    void updateCameraTarget(const domain::World &world, float delta_seconds);
    Vec3 computeFocusPoint(const domain::World &world) const;
    Vec3 computeCameraPosition(const Vec3 &focus) const;
    void applyViewMatrix(const Vec3 &eye, const Vec3 &target);
    void drawScene(const domain::World &world);
    void drawGround() const;
    void drawRoadSurface(float half_width) const;
    void drawVehicle(const domain::Vehicle &vehicle) const;
    void drawPedestrians(std::span<const domain::Pedestrian> pedestrians) const;
    void drawUnitCube() const;
    void drawPrism(float width, float height, float depth) const;
    static std::array<float, 16> makeLookAtMatrix(const Vec3 &eye, const Vec3 &target);

    unsigned int width_;
    unsigned int height_;
    std::unique_ptr<sf::RenderWindow> window_;
    bool window_active_{true};
    bool orbiting_{false};
    bool panning_{false};
    sf::Vector2i last_mouse_position_;
    Vec3 camera_target_offset_{};
    float camera_distance_{35.0F};
    float camera_yaw_deg_{-110.0F};
    float camera_pitch_deg_{35.0F};
    float camera_pan_speed_{20.0F};
    float keyboard_pan_speed_{20.0F};
    sf::Clock frame_clock_;
};
} // namespace ads::adapter