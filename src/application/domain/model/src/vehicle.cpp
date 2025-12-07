#include "vehicle.hpp"

#include <algorithm>
#include <cmath>

namespace sim::domain
{
Vehicle::Vehicle(const Vector2 &position, float heading_rad, float speed_mps, float max_speed_mps)
    : position_(position),
      heading_rad_(heading_rad),
      speed_mps_(speed_mps),
      target_speed_mps_(speed_mps),
      max_speed_mps_(std::max(max_speed_mps, 0.0F))
{
}

void Vehicle::setTargetSpeed(float target_speed_mps)
{
    target_speed_mps_ = std::clamp(target_speed_mps, 0.0F, max_speed_mps_);
}

void Vehicle::advance(float delta_seconds)
{
    if (delta_seconds <= 0.0F)
    {
        return;
    }

    const float speed_delta = target_speed_mps_ - speed_mps_;
    const float accel = speed_delta > 0.0F ? max_accel_mps2_ : -max_decel_mps2_;
    const float max_speed_change = accel * delta_seconds;

    if (std::abs(speed_delta) <= std::abs(max_speed_change))
    {
        speed_mps_ = target_speed_mps_;
    }
    else
    {
        speed_mps_ += max_speed_change;
    }

    const float dx = std::cos(heading_rad_) * speed_mps_ * delta_seconds;
    const float dy = std::sin(heading_rad_) * speed_mps_ * delta_seconds;
    position_.x += dx;
    position_.y += dy;
}
} // namespace sim::domain
