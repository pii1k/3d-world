#include "pedestrian.hpp"
#include <random>

namespace core
{
Pedestrian::Pedestrian(glm::vec3 position, float move_speed, glm::vec3 direction)
    : position_(position),
      move_speed_(move_speed),
      direction_(direction),
      random_engine(std::random_device),
      direction_distribution_(-1.0, 1.0),
      next_direction_change_time_(3.0f)
{
    direction_change_clock_.restart();
}

void Pedestrian::update(float delta_time)
{
    if (direction_change_clock_.getElapsedTime().asSeconds() >= next_direction_change_time_)
    {
        updateDirection();
        std::uniform_real_distribution<> time_distribution(2.0, 5.0);
        next_direction_change_time_ = time_distribution(random_engine_);
        direction_change_clock_.restart();
    }

    position_ += direction_ * move_speed_ * delta_time;
}

void Pedestrian::checkBounds(float map_half_size)
{
    if (position_.x > map_half_size)
    {
        position_.x = map_half_size;
        updateDirection();
    }
    if (position_.x < -map_half_size)
    {
        position_.x = -map_half_size;
        updateDirection();
    }
    if (position_.z > map_half_size)
    {
        position_.z = map_half_size;
        updateDirection();
    }
    if (position_.z < -map_half_size)
    {
        position_.z = -map_half_size;
        updateDirection();
    }
}

void Pedestrian::updateDirection()
{
    direction_.x = direction_distribution_(random_engine_);
    direction_.z = direction_distribution_(random_engine_);
    direction_.y = 0.0f;
    direction_ = glm::normalize(direction_);
}

} // namespace core