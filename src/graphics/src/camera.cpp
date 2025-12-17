#include <glm/ext/matrix_transform.hpp>
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

Camera::Camera(const glm::vec3 &position, const CameraConfig &config)
    : Camera(position, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), config)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    // 쿼터니언을 회전 행렬로 변환, 위치를 이동 행렬로 변환. view 행렬은 카메라 변환의 역행렬임
    glm::mat4 view_matrix = glm::mat4_cast(glm::conjugate(orientation_));
    view_matrix = glm::translate(view_matrix, -position_);
    return view_matrix;
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
    fov_ = fov_degrees;
    if (fov_ < 1.0f)
        fov_ = 1.0f;
    if (fov_ > 90.0f)
        fov_ = 90.0f;
}