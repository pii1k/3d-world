#pragma once

#include <cstddef>

namespace sim::application
{
class RunSimulationUseCase;
}

namespace sim::adapter
{
class SimulationCliAdapter
{
public:
    explicit SimulationCliAdapter(application::RunSimulationUseCase &use_case);

    void runDemo(std::size_t steps, float delta_seconds);

private:
    application::RunSimulationUseCase *use_case_;
};
} // namespace sim::adapter
