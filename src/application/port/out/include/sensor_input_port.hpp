#pragma once

#include "sensor_frame.hpp"

namespace ads::domain
{
class World;
}

namespace ads::application
{
class SensorInputPort
{
public:
    virtual ~SensorInputPort() = default;
    virtual SensorFrame getSensorFrame(const ads::domain::World &world) = 0;
};
} // namespace ads::application
