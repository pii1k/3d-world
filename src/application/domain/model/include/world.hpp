#pragma once

#include <span>
#include <vector>

#include "pedestrian.hpp"
#include "vehicle.hpp"

namespace ads::domain
{
class World
{
public:
    World(Vehicle ego_vehicle, std::vector<Pedestrian> pedestrians, float road_half_width);

    World(const World &) = delete;
    World &operator=(const World &) = delete;
    World(World &&) noexcept = default;
    World &operator=(World &&) noexcept = default;

    void advance(float delta_seconds);
    void setEgoTargetSpeed(float target_speed_mps);

    Vehicle &egoVehicle() noexcept { return ego_vehicle_; }
    const Vehicle &egoVehicle() const noexcept { return ego_vehicle_; }
    std::span<const Pedestrian> pedestrians() const noexcept { return pedestrians_; }
    float roadHalfWidth() const noexcept { return road_half_width_; }

private:
    void clampPedestrians();

    Vehicle ego_vehicle_;
    std::vector<Pedestrian> pedestrians_;
    float road_half_width_{5.0F};
};
} // namespace ads::domain
