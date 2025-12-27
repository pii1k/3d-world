#pragma once

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "component.hpp"
#include "world.hpp"

class ThirdPersonCameraSystem
{
public:
    void update(World &world, entity_id target_entity, entity_id camera_entity, float dt) const
    {
        auto target_comp = world.getComponent<TransformComponent>(target_entity);
        auto cam_comp = world.getComponent<CameraComponent>(camera_entity);
        auto camera_cmp = world.getComponent<ThirdPersonCameraComponent>(camera_entity);
        auto input_comp = world.getComponent<InputComponent>(target_entity);

        if (!target_comp || !cam_comp || !camera_cmp)
            return;

        auto &cam = camera_cmp->get();
        auto &camera = cam_comp->get();

        if (input_comp)
        {
            // Mouse right should rotate camera to the right (GLM's +Y angleAxis is CCW, so invert X).
            cam.yaw_deg -= input_comp->get().mouse_dx * cam.sensitivity_deg;
            cam.pitch_deg += input_comp->get().mouse_dy * cam.sensitivity_deg;
            cam.pitch_deg = std::clamp(cam.pitch_deg, cam.min_pitch_deg, cam.max_pitch_deg);

            cam.distance -= input_comp->get().scroll_y * cam.zoom_speed;
            cam.distance = std::clamp(cam.distance, cam.min_distance, cam.max_distance);

            input_comp->get().mouse_dx = 0.0f;
            input_comp->get().mouse_dy = 0.0f;
            input_comp->get().scroll_y = 0.0f;
        }

        const glm::vec3 pivot = target_comp->get().position + cam.pivot_offset;

        const float yaw_rad = glm::radians(cam.yaw_deg);
        const float pitch_rad = glm::radians(cam.pitch_deg);

        const glm::quat q_yaw = glm::angleAxis(yaw_rad, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::quat q_pitch = glm::angleAxis(pitch_rad, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::quat orientation = glm::normalize(q_yaw * q_pitch);

        const glm::vec3 forward = orientation * glm::vec3(0.0f, 0.0f, -1.0f);
        const glm::vec3 desired_pos = pivot - forward * cam.distance;

        if (!cam.enable_smoothing)
        {
            camera.position = desired_pos;
            camera.orientation = orientation;
            return;
        }

        // exp smoothing: a = 1 - exp(-k*dt)
        const float a = 1.0f - std::exp(-cam.smoothing_speed * dt);

        camera.position = glm::mix(camera.position, desired_pos, a);
        camera.orientation = glm::slerp(camera.orientation, orientation, a);
    };
};
