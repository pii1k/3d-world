#pragma once

namespace ads::application
{
class SimulationRunner;
}

namespace ads::adapter
{
class SimulationCliAdapter
{
public:
    explicit SimulationCliAdapter(application::SimulationRunner &simulation_runner);

    void runDemo();

private:
    application::SimulationRunner *simulation_runner_;
};
} // namespace ads::adapter
