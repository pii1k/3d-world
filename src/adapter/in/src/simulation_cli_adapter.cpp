#include <iostream>

#include "simulation_cli_adapter.hpp"
#include "simulation_runner.hpp"

namespace ads::adapter
{
SimulationCliAdapter::SimulationCliAdapter(application::SimulationRunner &simulation_runner)
    : simulation_runner_(&simulation_runner)
{
}

void SimulationCliAdapter::runDemo()
{
    std::cout << "Starting simulation. Close the window to exit.\n";
    simulation_runner_->run();
    std::cout << "Simulation finished.\n";
}
} // namespace ads::adapter
