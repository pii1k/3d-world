#pragma once

#include <vector>

#include "scenario_command.hpp"

namespace ads::domain
{
class World;
}

namespace ads::application
{
class ScenarioInputPort
{
public:
    virtual ~ScenarioInputPort() = default;
    virtual std::vector<ads::domain::ScenarioCommand> fetchCommands(const ads::domain::World &world) = 0;
};
} // namespace ads::application
