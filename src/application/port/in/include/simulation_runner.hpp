#pragma once

#include <cstddef>

namespace ads::domain
{
class World;
}

namespace ads::application
{
class SimulationRunner
{
public:
    virtual ~SimulationRunner() = default;

    virtual void runForSteps(std::size_t steps, float delta_seconds) = 0;
    virtual void tick(float delta_seconds) = 0;
    virtual const ads::domain::World &world() const = 0;
    virtual bool isRunning() const = 0;
};
} // namespace ads::application
