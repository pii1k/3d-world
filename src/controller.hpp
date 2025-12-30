#pragma once

#include "camera.hpp"

namespace contoller
{
class OrbitCamera
{
public:
    explicit OrbitCamera(Camera camera)
        : camera_(std::move(camera)) {}

    Camera &getCamera() { return camera_; }
    const Camera &getCamera() const { return camera_; }

    void update()
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

    void onScroll(double yoffset)
    {
        distance_ = glm::clamp(distance_ - static_cast<float>(yoffset) * zoom_speed_,
                               min_distance_,
                               max_distance_);
        this->update();
    }

private:
    Camera camera_;
    glm::vec3 target_{0.0f, 0.0f, 0.0f};
    float yaw_deg_ = 0.0f;
    float pitch_deg_ = 35.0f;
    float distance_ = 50.0f;
    float zoom_speed_ = 1.0f;
    float min_distance_ = 2.0f;
    float max_distance_ = 200.0f;
};

} // namespace contoller