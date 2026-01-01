#include "input/camera_controller.hpp"

void OrbitCamera::update()
{
    const float yaw = glm::radians(yaw_deg_);
    const float pitch = glm::radians(pitch_deg_);

    const float cosin_pitch = glm::cos(pitch);
    const glm::vec3 direction{cosin_pitch * glm::sin(yaw),
                              glm::sin(pitch),
                              cosin_pitch * glm::cos(yaw)};

    camera_.setTarget(target_);
    camera_.setPosition(target_ + direction * distance_);
    camera_.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void OrbitCamera::onScroll(double yoffset)
{
    distance_ = glm::clamp(distance_ - static_cast<float>(yoffset) * zoom_speed_,
                           min_distance_,
                           max_distance_);
    this->update();
}