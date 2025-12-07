#pragma once

#include <cstddef>

namespace sim::domain
{
class World;
}

namespace sim::application
{
class RunSimulationUseCase
{
public:
    virtual ~RunSimulationUseCase() = default;

    virtual void runForSteps(std::size_t steps, float delta_seconds) = 0;
    virtual void tick(float delta_seconds) = 0;
    virtual const sim::domain::World &world() const = 0;
};
} // namespace sim::application
