#include "simulation_service.hpp"

namespace ads::domain
{
SimulationService::SimulationService(World world) : world_(std::move(world)) {}

void SimulationService::step(float delta_seconds)
{
    if (delta_seconds <= 0.0F)
    {
        return;
    }

    world_.setEgoTargetSpeed(cruise_speed_mps_);

    world_.advance(delta_seconds);
}
} // namespace ads::domain
