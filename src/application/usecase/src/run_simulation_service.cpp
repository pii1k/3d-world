#include "run_simulation_service.hpp"

#include <algorithm>
#include <chrono>
#include <thread>
#include <utility>

#include "render_port.hpp"
#include "simulation_service.hpp"

namespace ads::application
{
RunSimulationService::RunSimulationService(domain::SimulationService simulation,
                                           RenderPort &render_port)
    : simulation_(std::move(simulation)),
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
    simulation_.step(delta_seconds);
}

const domain::World &RunSimulationService::world() const
{
    return simulation_.world();
}
} // namespace ads::application
