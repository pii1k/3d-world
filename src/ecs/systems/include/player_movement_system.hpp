#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "component.hpp"
#include "world.hpp"

class PlayerMovementSystem
{
public:
    void update(World &world, entity_id player, entity_id camera_entity, float delta_time) const
    {
        auto transform = world.getComponent<TransformComponent>(player);
        auto input = world.getComponent<InputComponent>(player);
        auto camera = world.getComponent<ThirdPersonCameraComponent>(camera_entity);
        if (!transform || !input || !camera)
            return;

        glm::vec3 direction{0.0f};
        if (input->get().move_forward)
            direction.z -= 1.0f;
        if (input->get().move_backward)
            direction.z += 1.0f;
        if (input->get().move_left)
            direction.x -= 1.0f;
        if (input->get().move_right)
            direction.x += 1.0f;

        if (glm::length(direction) == 0.0f)
            return;

        direction = glm::normalize(direction);

        const float yaw_rad = glm::radians(camera->get().yaw_deg);
        const glm::mat4 cam_rot = glm::rotate(glm::mat4(1.0f), yaw_rad, glm::vec3(0, 1, 0));
        glm::vec3 world_dir = glm::vec3(cam_rot * glm::vec4(direction, 0.0f));

        const float speed = input->get().sprint ? 1.0f : 0.5f;
        transform->get().position += world_dir * speed * delta_time;

        const float facing_yaw = glm::degrees(std::atan2(world_dir.x, -world_dir.z));
        transform->get().rotation = glm::angleAxis(glm::radians(facing_yaw), glm::vec3(0, 1, 0));
    }
};