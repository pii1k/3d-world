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

    virtual void run() = 0;
    virtual const ads::domain::World &world() const = 0;
};
} // namespace ads::application