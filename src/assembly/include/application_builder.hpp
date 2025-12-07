#pragma once

#include <memory>

#include "console_render_adapter.hpp"
#include "in_memory_v2x_adapter.hpp"
#include "run_simulation_use_case.hpp"
#include "scripted_sensor_adapter.hpp"
#include "simulation_cli_adapter.hpp"

namespace sim::assembly
{
struct SimulationApp
{
    std::unique_ptr<adapter::ConsoleRenderAdapter> render_adapter;
    std::unique_ptr<adapter::ScriptedSensorAdapter> sensor_adapter;
    std::unique_ptr<adapter::InMemoryV2XAdapter> v2x_adapter;
    std::unique_ptr<application::RunSimulationUseCase> use_case;
    std::unique_ptr<adapter::SimulationCliAdapter> controller;
};

class ApplicationBuilder
{
public:
    SimulationApp build();
};
} // namespace sim::assembly
