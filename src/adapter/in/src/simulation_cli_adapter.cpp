#include "simulation_cli_adapter.hpp"

#include <iostream>

#include "run_simulation_use_case.hpp"

namespace sim::adapter
{
SimulationCliAdapter::SimulationCliAdapter(application::RunSimulationUseCase &use_case)
    : use_case_(&use_case)
{
}

void SimulationCliAdapter::runDemo(std::size_t steps, float delta_seconds)
{
    std::cout << "Starting simulation (" << steps << " steps, dt=" << delta_seconds << "s)\n";
    use_case_->runForSteps(steps, delta_seconds);
    std::cout << "Simulation finished.\n";
}
} // namespace sim::adapter
