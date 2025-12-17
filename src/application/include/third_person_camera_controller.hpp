#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"
#include "camera_controller.hpp"
#include "registry.hpp"

// TODO(jyan): config는 추후에 따로 빼던가 해야할듯
struct ThirdPersonControllerConfig
{
    float distance_to_target = 5.0f;
    float initial_pitch = 20.0f;
    float initial_yaw = 0.0f;
    float mouse_sensitivity = 0.1f;
    float zoom_speed = 1.0f;
};

class ThirdPersonCameraController : public CameraController
{
public:
    ThirdPersonCameraController(Camera &camera,
                                Registry &registry,
                                Entity target_entity,
                                const ThirdPersonControllerConfig &config);

    void processMouseMovement(float xoffset, float yoffset) override;
    void processMouseScroll(float yoffset) override;

    void update(float deltaTime) override;

private:
    Camera &camera_;
    Registry &registry_;
    Entity target_entity_;

    float distance_;
    float pitch_;
    float yaw_;

    float sensitivity_;
    float zoom_speed_;
};