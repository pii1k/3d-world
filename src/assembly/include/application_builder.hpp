#pragma once

#include <memory>

#include "gl_render_adapter.hpp"
#include "scripted_sensor_adapter.hpp"
#include "simulation_cli_adapter.hpp"
#include "simulation_runner.hpp"
#include "v2x_adapter.hpp"

namespace ads::assembly
{
struct SimulationApp
{
    std::unique_ptr<adapter::GlRenderAdapter> render_adapter_ptr;
    std::unique_ptr<adapter::ScriptedSensorAdapter> sensor_adapter_ptr;
    std::unique_ptr<adapter::V2XAdapter> v2x_adapter_ptr;
    std::unique_ptr<application::SimulationRunner> simulation_runner_ptr;
    std::unique_ptr<adapter::SimulationCliAdapter> controller_ptr;
};

class ApplicationBuilder
{
public:
    SimulationApp build();
};
} // namespace ads::assembly
