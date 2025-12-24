#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "camera.hpp"

Camera::Camera(const glm::vec3 &position,
               const glm::quat &orientation,
               const CameraConfig &config)
    : position_(position),
      orientation_(orientation),
      fov_(config.fov),
      aspect_ratio_(config.aspect_ratio),
      near_plane_(config.near_plane),
      far_plane_(config.far_plane) {}

Camera::Camera(const glm::vec3 &position,
               const CameraConfig &config)
    : Camera(position,
             glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
             config) {}

glm::mat4 Camera::getViewMatrix() const
{
    // View = R^-1 * T^-1
    glm::mat4 R = glm::mat4_cast(glm::conjugate(orientation_));
    glm::mat4 T = glm::translate(glm::mat4(1.0f), -position_);
    return R * T;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov_), aspect_ratio_, near_plane_, far_plane_);
}

void Camera::setPosition(const glm::vec3 &position)
{
    position_ = position;
}

void Camera::setOrientation(const glm::quat &orientation)
{
    orientation_ = glm::normalize(orientation);
}

void Camera::setAspectRatio(float aspect_ratio)
{
    aspect_ratio_ = aspect_ratio;
}

void Camera::setFov(float fov_degrees)
{
    fov_ = std::clamp(fov_degrees, 1.0f, 90.f);
}

void Camera::setClipPlanes(float near_plane, float far_plane)
{
    near_plane_ = near_plane;
    far_plane_ = far_plane;
}

glm::vec3 Camera::forward() const
{
    return orientation_ * glm::vec3(0.0f, 0.0f, -1.0f); // -Z is forward in OpenGL convention
}

glm::vec3 Camera::right() const
{
    return orientation_ * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::up() const
{
    return orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::rotate(float yaw, float pitch)
{
    glm::quat yaw_rotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 right = glm::mat3_cast(orientation_) * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::quat pitch_rotation = glm::angleAxis(glm::radians(pitch), right);

    orientation_ = yaw_rotation * pitch_rotation * orientation_;
    orientation_ = glm::normalize(orientation_);
}

void Camera::zoom(float fov_degrees)
{
    setFov(fov_degrees);
}