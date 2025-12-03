#pragma once

#include "sensor-provider.hpp"

namespace adapter
{
class V2X : public interface::SensorProvider
{
public:
    std::vector<interface::SensedObject> getSensedObjects(const glm::vec3 &vehicle_position,
                                                          const glm::vec3 &vehicle_forward_vision,
                                                          const glm::vec3 &pedestrian_position) const override;
};
} // namespace adapter