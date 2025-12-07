#pragma once

#include <string>
#include <vector>

#include "vector2.hpp"

namespace sim::domain
{
struct PerceivedObstacle
{
    Vector2 position;
    float distance_m{0.0F};
};

struct SpeedAdvisory
{
    float recommended_speed_mps{0.0F};
    std::string source;
};

struct SimulationTickInput
{
    float delta_seconds{0.0F};
    std::vector<PerceivedObstacle> obstacles;
    std::vector<SpeedAdvisory> advisories;
};
} // namespace sim::domain
