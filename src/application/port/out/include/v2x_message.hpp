#pragma once

#include <string>

namespace sim::application
{
struct V2XMessage
{
    float recommended_speed_mps{0.0F};
    std::string note;
};
} // namespace sim::application
