#include "simulation_service.hpp"

#include <algorithm>

namespace ads::domain
{
SimulationService::SimulationService(World world) : world_(std::move(world)) {}

void SimulationService::step(const SimulationTickInput &input)
{
    if (input.delta_seconds <= 0.0F)
    {
        return;
    }

    world_.setEgoTargetSpeed(cruise_speed_mps_);
    adjustSpeedFromObstacles(input);
    applyAdvisories(input);

    world_.advance(input.delta_seconds);
}

void SimulationService::adjustSpeedFromObstacles(const SimulationTickInput &input)
{
    float target_speed = cruise_speed_mps_;
    const auto nearest = std::min_element(
        input.obstacles.begin(),
        input.obstacles.end(),
        [](const PerceivedObstacle &lhs, const PerceivedObstacle &rhs)
        { return lhs.distance_m < rhs.distance_m; });

    if (nearest != input.obstacles.end())
    {
        if (nearest->distance_m < 5.0F)
        {
            target_speed = 0.0F;
        }
        else if (nearest->distance_m < 15.0F)
        {
            target_speed = std::min(target_speed, cautious_speed_mps_);
        }
    }

    world_.setEgoTargetSpeed(target_speed);
}

void SimulationService::applyAdvisories(const SimulationTickInput &input)
{
    float current_target = world_.egoVehicle().targetSpeedMps();
    for (const auto &advisory : input.advisories)
    {
        current_target = std::min(current_target, advisory.recommended_speed_mps);
    }
    world_.setEgoTargetSpeed(current_target);
}
} // namespace ads::domain
