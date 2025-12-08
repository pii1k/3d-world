#include "v2x_adapter.hpp"

#include <cmath>
#include <optional>
#include <sstream>

#include "v2x_message.hpp"
#include "vector2.hpp"
#include "world.hpp"

namespace ads::adapter
{
V2XAdapter::V2XAdapter(float caution_distance_m) : caution_distance_m_(caution_distance_m) {}

std::vector<application::V2XMessage> V2XAdapter::receiveAll(const domain::World &world)
{
    std::vector<application::V2XMessage> messages;

    const auto &ego = world.egoVehicle();
    const domain::Vector2 forward{std::cos(ego.headingRad()), std::sin(ego.headingRad())};
    std::optional<float> nearest_ahead;

    for (const auto &ped : world.pedestrians())
    {
        const domain::Vector2 offset = ped.position() - ego.position();
        const float projection = domain::dot(offset, forward);
        if (projection < 0.0F)
        {
            continue;
        }

        const float lateral = std::abs(offset.x * forward.y - offset.y * forward.x);
        if (lateral > 3.0F)
        {
            continue;
        }

        if (!nearest_ahead || projection < *nearest_ahead)
        {
            nearest_ahead = projection;
        }
    }

    if (nearest_ahead && *nearest_ahead < caution_distance_m_)
    {
        std::ostringstream note;
        note << "pedestrian ahead at " << *nearest_ahead << "m";
        messages.push_back(application::V2XMessage{4.0F, note.str()});
    }

    return messages;
}
} // namespace ads::adapter
