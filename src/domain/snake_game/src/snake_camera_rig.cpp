#include "snake_camera_rig.hpp"

#include <glm/common.hpp>
#include <glm/trigonometric.hpp>

namespace domain
{
void SnakeCameraRig::init(int width, int height)
{
    CameraConfig cfg{};
    cfg.position = glm::vec3(0.0f, 2.0f, 12.0f);
    cfg.target = target_;
    cfg.up = glm::vec3(0.0f, 1.0f, 0.0f);
    cfg.fov_deg = fov_deg_;
    cfg.near_plane = near_plane_;
    cfg.far_plane = far_plane_;

    camera_ = std::make_unique<Camera>(cfg);
    onResize(width, height);
    update();
}

void SnakeCameraRig::update()
{
    if (!camera_)
        return;

    const float yaw = glm::radians(yaw_deg_);
    const float pitch = glm::radians(pitch_deg_);
    const float cos_pitch = glm::cos(pitch);
    const glm::vec3 direction{cos_pitch * glm::sin(yaw),
                              glm::sin(pitch),
                              cos_pitch * glm::cos(yaw)};

    camera_->setTarget(target_);
    camera_->setPosition(target_ + direction * distance_);
    camera_->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
    camera_->setPerspective(fov_deg_, near_plane_, far_plane_);
}

void SnakeCameraRig::onScroll(double yoffset)
{
    distance_ = glm::clamp(distance_ - static_cast<float>(yoffset) * zoom_speed_,
                           min_distance_,
                           max_distance_);
    update();
}

void SnakeCameraRig::onResize(int width, int height)
{
    if (!camera_)
        return;

    if (height < 1)
        height = 1;
    camera_->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}
} // namespace domain
