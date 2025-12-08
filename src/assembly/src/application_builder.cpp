#include "application_builder.hpp"

#include <utility>
#include <vector>

#include "gl_render_adapter.hpp"
#include "pedestrian.hpp"
#include "run_simulation_service.hpp"
#include "scripted_sensor_adapter.hpp"
#include "simulation_cli_adapter.hpp"
#include "simulation_service.hpp"
#include "v2x_adapter.hpp"
#include "vehicle.hpp"
#include "world.hpp"

namespace ads::assembly
{
SimulationApp ApplicationBuilder::build()
{
    domain::Vehicle ego({0.0F, 0.0F}, 0.0F, 0.0F, 22.0F);

    std::vector<domain::Pedestrian> pedestrians;
    pedestrians.emplace_back(domain::Vector2{18.0F, 1.5F}, domain::Vector2{-1.2F, 0.0F});
    pedestrians.emplace_back(domain::Vector2{25.0F, -1.0F}, domain::Vector2{-1.5F, 0.0F});

    domain::World world(std::move(ego), std::move(pedestrians), 4.0F);
    domain::SimulationService simulation(std::move(world));

    auto render_adapter_ptr = std::make_unique<adapter::GlRenderAdapter>(1280, 720);
    auto sensor_adapter_ptr = std::make_unique<adapter::ScriptedSensorAdapter>(25.0F, 100.0F);
    auto v2x_adapter_ptr = std::make_unique<adapter::V2XAdapter>(18.0F);
    auto simulation_runner_ptr = std::make_unique<application::RunSimulationService>(std::move(simulation),
                                                                                     *sensor_adapter_ptr,
                                                                                     *v2x_adapter_ptr,
                                                                                     *render_adapter_ptr);
    auto controller_ptr = std::make_unique<adapter::SimulationCliAdapter>(*simulation_runner_ptr);

    SimulationApp app{
        std::move(render_adapter_ptr),
        std::move(sensor_adapter_ptr),
        std::move(v2x_adapter_ptr),
        std::move(simulation_runner_ptr),
        std::move(controller_ptr)};

    return app;
}
} // namespace ads::assembly
