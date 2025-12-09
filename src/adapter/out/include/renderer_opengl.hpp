#pragma once

#include <array>
#include <memory>
#include <span>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "controller_opengl.hpp"
#include "pedestrian.hpp"
#include "render_port.hpp"
#include "vehicle.hpp"

namespace ads::adapter
{
class GlRenderAdapter : public application::RenderPort
{
public:
    explicit GlRenderAdapter(ControllerOpengl &controller);

    void render(const domain::World &world) override;
    bool isActive() const override;

private:
    void ensureGlState();
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

    ControllerOpengl *controller_;
    bool initialized_{false};
    float keyboard_pan_speed_{20.0F};
    sf::Clock frame_clock_;
};
} // namespace ads::adapter
