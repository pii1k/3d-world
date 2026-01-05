#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "camera.hpp"

class CameraSystem
{
public:
    virtual ~CameraSystem() = default;
    virtual void setPivot(const glm::vec3 &pivot) {}
    virtual void update(float &mouse_dx,
                        float &mouse_dy,
                        float &scroll_y,
                        Camera &camera) = 0;
};

struct QuarterViewCameraState
{
    float yaw_deg = 45.0f;
    float pitch_deg = -35.0f;
    float distance = 12.0f;

    float zoom_speed = 1.5f;
    float min_distance = 6.0f;
    float max_distance = 30.0f;

    glm::vec3 pivot = {0.0f, 1.6f, 0.0f};
};

class QuarterViewCameraSystem : public CameraSystem
{
public:
    void setPivot(const glm::vec3 &pivot) { state_.pivot = pivot; }

    void update(float & /* mouse_dx */,
                float & /* mouse_dy */,
                float &scroll_y,
                Camera &camera)
    {
        state_.distance -= scroll_y * state_.zoom_speed;
        state_.distance = std::clamp(state_.distance, state_.min_distance, state_.max_distance);

        const glm::quat yaw_q = glm::angleAxis(glm::radians(state_.yaw_deg), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::vec3 right_axis = yaw_q * glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::quat pitch_q = glm::angleAxis(glm::radians(state_.pitch_deg), right_axis);
        const glm::quat orientation = glm::normalize(pitch_q * yaw_q);

        const glm::vec3 backward = orientation * glm::vec3(0.0f, 0.0f, 1.0f);
        const glm::vec3 position = state_.pivot + backward * state_.distance;

        camera.setOrientation(orientation);
        camera.setPosition(position);
    }

private:
    QuarterViewCameraState state_{};
};

struct OrbitCameraState
{
    float yaw_deg = 0.0f;
    float pitch_deg = -15.0f;
    float distance = 6.0f;

    float sensitivity_deg = 0.02f;
    float zoom_speed = 0.7f;

    float min_pitch_deg = -75.0f;
    float max_pitch_deg = 20.0f;
    float min_distance = 1.5f;
    float max_distance = 25.0f;

    glm::vec3 pivot = {0.0f, 1.6f, 0.0f};
};

class OrbitCameraSystem : public CameraSystem
{
public:
    void update(float &mouse_dx,
                float &mouse_dy,
                float &scroll_y,
                Camera &camera)
    {
        state_.yaw_deg += mouse_dx * state_.sensitivity_deg;
        state_.pitch_deg += mouse_dy * state_.sensitivity_deg;
        state_.pitch_deg = std::clamp(state_.pitch_deg, state_.min_pitch_deg, state_.max_pitch_deg);

        state_.distance -= scroll_y * state_.zoom_speed;
        state_.distance = std::clamp(state_.distance, state_.min_distance, state_.max_distance);

        const glm::quat yaw_q = glm::angleAxis(glm::radians(state_.yaw_deg), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::vec3 right_axis = yaw_q * glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::quat pitch_q = glm::angleAxis(glm::radians(state_.pitch_deg), right_axis);
        const glm::quat orientation = glm::normalize(pitch_q * yaw_q);

        const glm::vec3 backward = orientation * glm::vec3(0.0f, 0.0f, 1.0f);
        const glm::vec3 position = state_.pivot + backward * state_.distance;

        camera.setOrientation(orientation);
        camera.setPosition(position);

        mouse_dx = 0.0f;
        mouse_dy = 0.0f;
        scroll_y = 0.0f;
    }

private:
    OrbitCameraState state_{};
};