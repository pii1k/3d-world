#pragma once

#include <cstddef>
#include <memory>

#include "run_simulation_use_case.hpp"
#include "simulation_service.hpp"

namespace sim::domain
{
class SimulationService;
}

namespace sim::application
{
class SensorInputPort;
class V2XCommPort;
class RenderPort;

class RunSimulationService : public RunSimulationUseCase
{
public:
    RunSimulationService(domain::SimulationService simulation,
                         SensorInputPort &sensor_port,
                         V2XCommPort &v2x_port,
                         RenderPort &render_port);

    RunSimulationService(const RunSimulationService &) = delete;
    RunSimulationService &operator=(const RunSimulationService &) = delete;
    RunSimulationService(RunSimulationService &&) noexcept = default;
    RunSimulationService &operator=(RunSimulationService &&) noexcept = default;

    void runForSteps(std::size_t steps, float delta_seconds) override;
    void tick(float delta_seconds) override;
    const domain::World &world() const override;

private:
    domain::SimulationService simulation_;
    SensorInputPort *sensor_port_;
    V2XCommPort *v2x_port_;
    RenderPort *render_port_;
};
} // namespace sim::application
