#include "application_builder.hpp"

#include <iostream>
#include <utility>
#include <vector>

#include "console_render_adapter.hpp"
#include "in_memory_v2x_adapter.hpp"
#include "run_simulation_service.hpp"
#include "simulation_cli_adapter.hpp"
#include "simulation_service.hpp"
#include "vehicle.hpp"
#include "pedestrian.hpp"
#include "scripted_sensor_adapter.hpp"
#include "world.hpp"

namespace sim::assembly
{
SimulationApp ApplicationBuilder::build()
{
    domain::Vehicle ego({0.0F, 0.0F}, 0.0F, 0.0F, 22.0F);

    std::vector<domain::Pedestrian> pedestrians;
    pedestrians.emplace_back(domain::Vector2{18.0F, 1.5F}, domain::Vector2{-1.2F, 0.0F});
    pedestrians.emplace_back(domain::Vector2{25.0F, -1.0F}, domain::Vector2{-1.5F, 0.0F});

    domain::World world(std::move(ego), std::move(pedestrians), 4.0F);
    domain::SimulationService simulation(std::move(world));

    auto render_adapter = std::make_unique<adapter::ConsoleRenderAdapter>(std::cout);
    auto sensor_adapter = std::make_unique<adapter::ScriptedSensorAdapter>(25.0F, 100.0F);
    auto v2x_adapter = std::make_unique<adapter::InMemoryV2XAdapter>(18.0F);

    auto use_case = std::make_unique<application::RunSimulationService>(std::move(simulation),
                                                                        *sensor_adapter,
                                                                        *v2x_adapter,
                                                                        *render_adapter);
    auto controller = std::make_unique<adapter::SimulationCliAdapter>(*use_case);

    SimulationApp app{
        std::move(render_adapter),
        std::move(sensor_adapter),
        std::move(v2x_adapter),
        std::move(use_case),
        std::move(controller)};

    return app;
}
} // namespace sim::assembly
