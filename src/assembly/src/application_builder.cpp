#include "application_builder.hpp"

#include <utility>
#include <vector>

#include "renderer_opengl.hpp"
#include "pedestrian.hpp"
#include "run_simulation_service.hpp"
#include "simulation_service.hpp"
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

    auto controller_ptr = std::make_unique<adapter::ControllerOpengl>(1280, 720);
    auto render_adapter_ptr = std::make_unique<adapter::GlRenderAdapter>(*controller_ptr);
    auto simulation_runner_ptr = std::make_unique<application::RunSimulationService>(std::move(simulation), *render_adapter_ptr);

    SimulationApp app{std::move(controller_ptr),
                      std::move(render_adapter_ptr),
                      std::move(simulation_runner_ptr)};

    return app;
}
} // namespace ads::assembly
