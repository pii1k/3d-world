#include "run_simulation_service.hpp"

#include <algorithm>
#include <chrono>
#include <thread>
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

void RunSimulationService::run()
{
    using clock = std::chrono::steady_clock;
    auto previous_time = clock::now();
    float accumulator = 0.0F;
    constexpr float kMaxAccumulator = 0.25F;

    while (render_port_->isActive())
    {
        const auto now = clock::now();
        float frame_time = std::chrono::duration<float>(now - previous_time).count();
        previous_time = now;
        frame_time = std::clamp(frame_time, 0.0F, 0.1F);
        accumulator = std::min(accumulator + frame_time, kMaxAccumulator);

        while (accumulator >= fixed_delta_seconds_)
        {
            stepSimulation(fixed_delta_seconds_);
            accumulator -= fixed_delta_seconds_;
        }

        render_port_->render(simulation_.world());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void RunSimulationService::stepSimulation(float delta_seconds)
{
    if (delta_seconds <= 0.0F)
    {
        return;
    }

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
}

const domain::World &RunSimulationService::world() const
{
    return simulation_.world();
}
} // namespace ads::application
