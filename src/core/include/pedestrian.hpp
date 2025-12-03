#pragma once

#include <SFML/System/Clock.hpp>
#include <glm/glm.hpp>
#include <random>

namespace core
{
class Pedestrian
{
public:
    Pedestrian(glm::vec3 position, float move_speed, glm::vec3 direction);

    void update(float delta_time);
    void checkBounds(float map_half_size);
    const glm::vec3 &getPosition() const { return position_; };

private:
    glm::vec3 position_;
    glm::vec3 direction_;
    float move_speed_;
    std::mt19937 random_engine_;
    std::uniform_real_distribution<> direction_distribution_;
    sf::Clock direction_change_clock_;
    float next_direction_change_time_;

    void updateDirection();
};
} // namespace core