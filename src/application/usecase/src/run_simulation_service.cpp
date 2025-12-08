#include "run_simulation_service.hpp"

#include <utility>

#include "render_port.hpp"
#include "sensor_input_port.hpp"
#include "simulation_service.hpp"
#include "simulation_types.hpp"
#include "v2x_comm_port.hpp"

namespace ads::application
{
RunSimulationService::RunSimulationService(domain::SimulationService simulation,
                                           SensorInputPort &sensor_port,
                                           V2XCommPort &v2x_port,
                                           RenderPort &render_port)
    : simulation_(std::move(simulation)),
      sensor_port_(&sensor_port),
      v2x_port_(&v2x_port),
      render_port_(&render_port)
{
}

void RunSimulationService::runForSteps(std::size_t steps, float delta_seconds)
{
    for (std::size_t i = 0; i < steps; ++i)
    {
        if (!isRunning())
        {
            break;
        }
        tick(delta_seconds);
    }
}

void RunSimulationService::tick(float delta_seconds)
{
    domain::SimulationTickInput input{};
    input.delta_seconds = delta_seconds;

    auto sensor_frame = sensor_port_->getSensorFrame(simulation_.world());
    input.obstacles.reserve(sensor_frame.objects.size());
    for (const auto &object : sensor_frame.objects)
    {
        input.obstacles.push_back(domain::PerceivedObstacle{object.position, object.distance_m});
    }

    auto v2x_messages = v2x_port_->receiveAll(simulation_.world());
    input.advisories.reserve(v2x_messages.size());
    for (const auto &message : v2x_messages)
    {
        input.advisories.push_back(domain::SpeedAdvisory{message.recommended_speed_mps, message.note});
    }

    simulation_.step(input);
    render_port_->render(simulation_.world());
}

const domain::World &RunSimulationService::world() const
{
    return simulation_.world();
}

bool RunSimulationService::isRunning() const
{
    return render_port_->isActive();
}
} // namespace ads::application
