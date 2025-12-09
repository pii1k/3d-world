#pragma once

#include "world.hpp"

namespace ads::domain
{
class SimulationService
{
public:
    explicit SimulationService(World world);

    SimulationService(const SimulationService &) = delete;
    SimulationService &operator=(const SimulationService &) = delete;
    SimulationService(SimulationService &&) noexcept = default;
    SimulationService &operator=(SimulationService &&) noexcept = default;

    void step(float delta_seconds);

    const World &world() const noexcept { return world_; }
    World &world() noexcept { return world_; }

private:
    World world_;
    float cruise_speed_mps_{12.0F};
};
} // namespace ads::domain
