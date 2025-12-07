#pragma once

#include <vector>

#include "simulation_types.hpp"
#include "world.hpp"

namespace sim::domain
{
class SimulationService
{
public:
    explicit SimulationService(World world);

    SimulationService(const SimulationService &) = delete;
    SimulationService &operator=(const SimulationService &) = delete;
    SimulationService(SimulationService &&) noexcept = default;
    SimulationService &operator=(SimulationService &&) noexcept = default;

    void step(const SimulationTickInput &input);

    const World &world() const noexcept { return world_; }
    World &world() noexcept { return world_; }

private:
    void adjustSpeedFromObstacles(const SimulationTickInput &input);
    void applyAdvisories(const SimulationTickInput &input);

    World world_;
    float cruise_speed_mps_{12.0F};
    float cautious_speed_mps_{5.0F};
};
} // namespace sim::domain
