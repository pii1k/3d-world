#pragma once

#include "camera.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using entity_id = std::uint32_t;

struct TransformComponent
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 getTransform() const
    {
        glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
        return glm::translate(glm::mat4(1.0f), position) * rotation_matrix * glm::scale(glm::mat4(1.0f), scale);
    }
};

struct RenderableComponent
{
    int mesh_id = 0;
    glm::vec3 color{1.0f, 0.5f, 0.2f};
    bool use_grid = false;
};

enum class LightType
{
    Directional,
    Point,
    Spot,
};

struct LightComponent
{
    LightType type{LightType::Directional};
    glm::vec3 color{1.0f};
    float intensity{1.0f};
    glm::vec3 position{0.0f};
    glm::vec3 direction{0, -1, 0};
    float range{15.0f};
    float inner_cone{0.85f};
    float outer_cone{0.9f};
    bool enabled{true};
};

struct PhysicsComponent
{
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f);
    float friction = 0.95f;
    bool grounded = false;
};

struct InputComponent
{
    bool move_forward = false;
    bool move_backward = false;
    bool move_left = false;
    bool move_right = false;

    bool sprint = false;
    bool interact = false;
    bool jump = false;

    float mouse_dx = 0.0f;
    float mouse_dy = 0.0f;
    float scroll_y = 0.0f;
};

struct CameraComponent
{
    glm::vec3 position{0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
    CameraConfig config{};
};

struct ThirdPersonCameraConfig
{
    float mouse_sensitivity = 0.15f; // deg per pixel
    float zoom_speed = 1.0f;         // distance per scroll step

    float min_distance = 1.5f;
    float max_distance = 10.0f;

    float min_pitch_deg = -75.0f;
    float max_pitch_deg = 20.0f;

    glm::vec3 pivot_offset = {0.0f, 1.6f, 0.0f};
};

struct ThirdPersonCameraComponent
{
    // ---- runtime state (source of truth) ----
    float yaw_deg = 0.0f;     // left/right
    float pitch_deg = -15.0f; // up/down
    float distance = 4.0f;    // zoom (orbit radius)

    // ---- config / tuning ----
    float sensitivity_deg = 0.02f; // degrees per pixel
    float zoom_speed = 0.7f;       // distance per scroll step

    float min_pitch_deg = -75.0f;
    float max_pitch_deg = 20.0f;

    float min_distance = 1.5f;
    float max_distance = 10.0f;

    // pivot (target position + offset) e.g. head/chest height
    glm::vec3 pivot_offset = {0.0f, 1.6f, 0.0f};

    // (선택) smoothing
    bool enable_smoothing = false;
    float smoothing_speed = 12.0f; // exp smoothing gain
};