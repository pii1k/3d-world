#include "engine/camera.hpp"

#include <algorithm>

Camera::Camera(const CameraConfig &cfg)
    : position_(cfg.position),
      target_(cfg.target),
      up_(cfg.up),
      fov_deg_(cfg.fov_deg),
      near_plane_(cfg.near_plane),
      far_plane_(cfg.far_plane),
      use_ortho_(cfg.use_ortho),
      ortho_size_(cfg.ortho_size) {}

glm::mat4 Camera::view() const
{
    return glm::lookAt(position_, target_, up_);
}

glm::mat4 Camera::proj(int w, int h) const
{
    const float aspect_ratio = static_cast<float>(w) / static_cast<float>(std::max(1, h));
    return use_ortho_ ? glm::ortho(-ortho_size_ * aspect_ratio, ortho_size_ * aspect_ratio,
                                   -ortho_size_, ortho_size_, near_plane_, far_plane_)
                      : glm::perspective(glm::radians(fov_deg_), aspect_ratio, near_plane_, far_plane_);
}

void Camera::setPosition(const glm::vec3 &pos)
{
    position_ = pos;
}

void Camera::setTarget(const glm::vec3 &target)
{
    target_ = target;
}

void Camera::setUp(const glm::vec3 &up)
{
    up_ = up;
}

void Camera::setPerspective(float fov_deg, float near_plane, float far_plane)
{
    use_ortho_ = false;
    fov_deg_ = fov_deg;
    near_plane_ = near_plane;
    far_plane_ = far_plane;
}

void Camera::setOrtho(float size, float near_plane, float far_plane)
{
    use_ortho_ = true;
    ortho_size_ = size;
    near_plane_ = near_plane;
    far_plane_ = far_plane;
}