#pragma once

#include "sensor_frame.hpp"

namespace sim::domain
{
class World;
}

namespace sim::application
{
class SensorInputPort
{
public:
    virtual ~SensorInputPort() = default;
    virtual SensorFrame getSensorFrame(const sim::domain::World &world) = 0;
};
} // namespace sim::application
