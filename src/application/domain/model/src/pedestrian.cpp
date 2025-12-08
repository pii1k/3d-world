#include "pedestrian.hpp"

namespace ads::domain
{
Pedestrian::Pedestrian(const Vector2 &position, const Vector2 &velocity)
    : position_(position), velocity_(velocity)
{
}

void Pedestrian::advance(float delta_seconds)
{
    if (delta_seconds <= 0.0F)
    {
        return;
    }
    position_.x += velocity_.x * delta_seconds;
    position_.y += velocity_.y * delta_seconds;
}
} // namespace ads::domain
