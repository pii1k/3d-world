#pragma once

#include <memory>

#include "gl_render_adapter.hpp"
#include "simulation_runner.hpp"

namespace ads::assembly
{
struct SimulationApp
{
    std::unique_ptr<adapter::GlRenderAdapter> render_adapter_ptr;
    std::unique_ptr<application::SimulationRunner> simulation_runner_ptr;
};

class ApplicationBuilder
{
public:
    SimulationApp build();
};
} // namespace ads::assembly
