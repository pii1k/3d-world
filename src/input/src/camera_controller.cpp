#include "camera_controller.hpp"

namespace controller
{
CameraController::CameraController(const CameraConfig &cam_cfg)
    : camera_(std::make_unique<Camera>(cam_cfg)),
      target_(cam_cfg.target)
{
}

void CameraController::update()
{
    const float yaw = glm::radians(yaw_deg_);
    const float pitch = glm::radians(pitch_deg_);

    const float cosin_pitch = glm::cos(pitch);
    const glm::vec3 direction{cosin_pitch * glm::sin(yaw),
                              glm::sin(pitch),
                              cosin_pitch * glm::cos(yaw)};

    camera_->setTarget(target_);
    camera_->setPosition(target_ + direction * distance_);
    camera_->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void CameraController::onScroll(double yoffset)
{
    distance_ = glm::clamp(distance_ - static_cast<float>(yoffset) * zoom_speed_,
                           min_distance_,
                           max_distance_);
    this->update();
}

void CameraController::onResize(int width, int height)
{
    if (height < 1)
        height = 1;
    camera_->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}
} // namespace controller