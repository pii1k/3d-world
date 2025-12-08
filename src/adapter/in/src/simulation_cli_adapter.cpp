#include <chrono>
#include <iostream>
#include <thread>

#include "simulation_cli_adapter.hpp"
#include "simulation_runner.hpp"

namespace ads::adapter
{
SimulationCliAdapter::SimulationCliAdapter(application::SimulationRunner &simulation_runner)
    : simulation_runner_(&simulation_runner)
{
}

void SimulationCliAdapter::runDemo(std::size_t steps, float delta_seconds)
{
    std::cout << "Starting simulation (dt=" << delta_seconds << "s). Close the window to exit.\n";

    std::size_t executed = 0;
    const bool has_limit = steps > 0;

    while (simulation_runner_->isRunning())
    {
        const bool limit_reached = has_limit && executed >= steps;
        const float dt = limit_reached ? 0.0F : delta_seconds;
        simulation_runner_->tick(dt);
        if (!limit_reached)
        {
            ++executed;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Simulation finished.\n";
}
} // namespace ads::adapter
