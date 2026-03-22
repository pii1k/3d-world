#include "bat_camera_rig.hpp"

#include <algorithm>
#include <glm/trigonometric.hpp>

namespace domain
{
void BatCameraRig::init(int width, int height)
{
    CameraConfig cfg{};
    cfg.position = glm::vec3(0.0f, 4.0f, 12.0f);
    cfg.target = glm::vec3(0.0f, 2.0f, 0.0f);
    cfg.up = glm::vec3(0.0f, 1.0f, 0.0f);
    cfg.fov_deg = base_fov_deg_;
    cfg.near_plane = 0.1f;
    cfg.far_plane = 300.0f;

    camera_ = std::make_unique<Camera>(cfg);

    if (width > 0 && height > 0)
        camera_->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    smooth_pos_ = cfg.position;
}

void BatCameraRig::update(float dt_s, const BatState &bat)
{
    if (!camera_)
        return;

    const float yaw = glm::radians(bat.camera_yaw_deg);
    const float pitch = glm::radians(bat.camera_pitch_deg);
    const float cos_pitch = glm::cos(pitch);
    const glm::vec3 look_dir{cos_pitch * glm::sin(yaw),
                             glm::sin(pitch),
                             cos_pitch * glm::cos(yaw)};
    const glm::vec3 target = bat.position + glm::vec3(0.0f, height_, 0.0f);
    const glm::vec3 desired = target - look_dir * distance_;
    const float t = 1.0f - std::exp(-smooth_rate_ * std::max(0.0f, dt_s));
    smooth_pos_ = glm::mix(smooth_pos_, desired, t);

    camera_->setPosition(smooth_pos_);
    camera_->setTarget(target);
    camera_->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
    camera_->setPerspective(base_fov_deg_, 0.1f, 300.0f);
}

void BatCameraRig::onResize(int width, int height)
{
    if (!camera_)
        return;

    if (width > 0 && height > 0)
        camera_->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}
} // namespace domain
