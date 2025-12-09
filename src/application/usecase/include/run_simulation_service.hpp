#pragma once

#include <cstddef>

#include "simulation_runner.hpp"
#include "simulation_service.hpp"

namespace ads::domain
{
class SimulationService;
}

namespace ads::application
{
class RenderPort;

class RunSimulationService : public SimulationRunner
{
public:
    RunSimulationService(domain::SimulationService simulation,
                         RenderPort &render_port);

    RunSimulationService(const RunSimulationService &) = delete;
    RunSimulationService &operator=(const RunSimulationService &) = delete;
    RunSimulationService(RunSimulationService &&) noexcept = default;
    RunSimulationService &operator=(RunSimulationService &&) noexcept = default;

    void run() override;
    const domain::World &world() const override;

private:
    void stepSimulation(float delta_seconds);

    domain::SimulationService simulation_;
    RenderPort *render_port_;
    float fixed_delta_seconds_{1.0F / 60.0F};
};
} // namespace ads::application
