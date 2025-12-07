#pragma once

#include "sensor_input_port.hpp"

namespace sim::adapter
{
class ScriptedSensorAdapter : public application::SensorInputPort
{
public:
    ScriptedSensorAdapter(float detection_range_m, float field_of_view_deg);

    application::SensorFrame getSensorFrame(const domain::World &world) override;

private:
    float detection_range_m_{20.0F};
    float field_of_view_rad_{1.57F};
};
} // namespace sim::adapter
