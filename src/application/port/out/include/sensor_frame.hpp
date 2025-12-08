#pragma once

#include <vector>

#include "vector2.hpp"

namespace ads::application
{
struct SensedObject
{
    ads::domain::Vector2 position;
    float distance_m{0.0F};
};

struct SensorFrame
{
    std::vector<SensedObject> objects;
};
} // namespace ads::application
