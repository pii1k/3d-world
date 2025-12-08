#pragma once

#include "vector2.hpp"

namespace ads::domain
{
class Vehicle
{
public:
    Vehicle(const Vector2 &position, float heading_rad, float speed_mps, float max_speed_mps);

    Vehicle(const Vehicle &) = delete;
    Vehicle &operator=(const Vehicle &) = delete;
    Vehicle(Vehicle &&) noexcept = default;
    Vehicle &operator=(Vehicle &&) noexcept = default;

    void setTargetSpeed(float target_speed_mps);
    void advance(float delta_seconds);

    const Vector2 &position() const noexcept { return position_; }
    float headingRad() const noexcept { return heading_rad_; }
    float speedMps() const noexcept { return speed_mps_; }
    float targetSpeedMps() const noexcept { return target_speed_mps_; }

private:
    Vector2 position_;
    float heading_rad_{0.0F};
    float speed_mps_{0.0F};
    float target_speed_mps_{0.0F};
    float max_accel_mps2_{3.0F};
    float max_decel_mps2_{5.0F};
    float max_speed_mps_{20.0F};
};
} // namespace ads::domain
