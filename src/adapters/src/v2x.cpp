#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "v2x.hpp"

namespace adapter
{
std::vector<interface::SensedObject> V2X::getSensedObjects(const glm::vec3 & /* vehicle_position */,
                                                           const glm::vec3 & /* vehicle_forward_vector */,
                                                           const glm::vec3 &pedestrian_position) const
{
    std::vector<interface::SensedObject> sensed_objects;
    sensed_objects.push_back({pedestrian_position});

    return sensed_objects;
}
} // namespace adapter