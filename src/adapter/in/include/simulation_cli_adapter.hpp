#pragma once

#include <cstddef>

namespace ads::application
{
class SimulationRunner;
}

namespace ads::adapter
{
class SimulationCliAdapter
{
public:
    explicit SimulationCliAdapter(application::SimulationRunner &simulation_runner);

    void runDemo(std::size_t steps, float delta_seconds);

private:
    application::SimulationRunner *simulation_runner_;
};
} // namespace ads::adapter
