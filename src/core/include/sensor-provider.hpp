#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace interface
{
struct SensedObject
{
    glm::vec3 position;
};

class SensorProvider
{
public:
    virtual ~SensorProvider() = default;
    virtual std::vector<SensedObject> getSensedObjects(const glm::vec3 &vehicle_position,
                                                       const glm::vec3 &vehicle_forward_vector,
                                                       const glm::vec3 &pedestrian_position) const = 0;
};
} // namespace interface