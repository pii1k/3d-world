#pragma once

#include <vector>

#include "vector2.hpp"

namespace sim::application
{
struct SensedObject
{
    sim::domain::Vector2 position;
    float distance_m{0.0F};
};

struct SensorFrame
{
    std::vector<SensedObject> objects;
};
} // namespace sim::application
