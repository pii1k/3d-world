#include "scripted_sensor_adapter.hpp"

#include <cmath>

#include "sensor_frame.hpp"
#include "vector2.hpp"
#include "world.hpp"

namespace ads::adapter
{
ScriptedSensorAdapter::ScriptedSensorAdapter(float detection_range_m, float field_of_view_deg)
    : detection_range_m_(detection_range_m)
{
    constexpr float kPi = 3.1415926535F;
    field_of_view_rad_ = field_of_view_deg * kPi / 180.0F;
}

application::SensorFrame ScriptedSensorAdapter::getSensorFrame(const domain::World &world)
{
    application::SensorFrame frame;
    const auto &ego = world.egoVehicle();
    const domain::Vector2 forward{std::cos(ego.headingRad()), std::sin(ego.headingRad())};
    const float cos_threshold = std::cos(field_of_view_rad_ * 0.5F);

    for (const auto &ped : world.pedestrians())
    {
        const domain::Vector2 offset = ped.position() - ego.position();
        const float distance_m = domain::length(offset);
        if (distance_m > detection_range_m_)
        {
            continue;
        }

        const domain::Vector2 direction = domain::normalize(offset);
        const float facing_alignment = domain::dot(direction, forward);
        if (facing_alignment >= cos_threshold)
        {
            frame.objects.push_back(application::SensedObject{ped.position(), distance_m});
        }
    }

    return frame;
}
} // namespace ads::adapter
