#pragma once

#include "vector2.hpp"

namespace ads::domain
{
class Pedestrian
{
public:
    Pedestrian(const Vector2 &position, const Vector2 &velocity);

    Pedestrian(const Pedestrian &) = delete;
    Pedestrian &operator=(const Pedestrian &) = delete;
    Pedestrian(Pedestrian &&) noexcept = default;
    Pedestrian &operator=(Pedestrian &&) noexcept = default;

    void advance(float delta_seconds);

    const Vector2 &position() const noexcept { return position_; }
    const Vector2 &velocity() const noexcept { return velocity_; }

private:
    Vector2 position_;
    Vector2 velocity_;
};
} // namespace ads::domain
