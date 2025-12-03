#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "vision.hpp"

namespace adapter
{
std::vector<interface::SensedObject> Vision::getSensedObjects(const glm::vec3 &vehicle_position,
                                                              const glm::vec3 &vehicle_forward_vector,
                                                              const glm::vec3 &pedestrian_position) const
{
    std::vector<interface::SensedObject> sensed_objects;
    glm::vec3 to_pedestrian = pedestrian_position - vehicle_position;
    float dist = glm::length(to_pedestrian);
    float fov = glm::radians(90.0f);
    float view_dist = 10.0f;

    if (dist < view_dist)
    {
        glm::vec3 to_pedestrian_norm = glm::normalize(to_pedestrian);
        float dot_product = glm::dot(vehicle_forward_vector, to_pedestrian_norm);
        float angle = glm::acos(dot_product);
        if (angle < fov / 2.0f)
            sensed_objects.push_back({pedestrian_position});
    }

    return sensed_objects;
}
} // namespace adapter