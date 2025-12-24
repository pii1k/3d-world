#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"
#include "camera_controller.hpp"
#include "world.hpp"

// TODO(jyan): config는 추후에 따로 빼던가 해야할듯
struct ThirdPersonControllerConfig
{
    float mouse_sensitivity = 0.15f; // deg per pixel
    float zoom_speed = 1.0f;         // distance per scroll step

    float min_distance = 1.5f;
    float max_distance = 10.0f;

    float min_pitch_deg = -75.0f;
    float max_pitch_deg = 20.0f;

    glm::vec3 pivot_offset = {0.0f, 1.6f, 0.0f};
};

struct ThirdPersonControllerState
{
    entity_id target{};
    float yaw_deg = 0.0f;
    float pitch_deg = -15.0f;
    float distance = 4.0f;
};

class ThirdPersonCameraController : public CameraController
{
public:
    ThirdPersonCameraController(Camera &camera,
                                World &world,
                                entity_id target_entity,
                                const ThirdPersonControllerConfig &config);

    void processMouseMovement(float offset_x, float offset_y) override;
    void processMouseScroll(float offset_y) override;

    void update(float deltaTime) override;

private:
    Camera &camera_;
    World &world_;
    ThirdPersonControllerState controller_state_;
    ThirdPersonControllerConfig controller_config_;
};