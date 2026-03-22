#pragma once

#include "camera.hpp"

#include <memory>

namespace domain
{
class SnakeCameraRig
{
public:
    void init(int width, int height);
    void update();
    void onScroll(double yoffset);
    void onResize(int width, int height);

    Camera &getCamera() { return *camera_; }
    const Camera &getCamera() const { return *camera_; }

private:
    std::unique_ptr<Camera> camera_;
    glm::vec3 target_{0.0f, 0.0f, 0.0f};
    float yaw_deg_ = 0.0f;
    float pitch_deg_ = 35.0f;
    float distance_ = 50.0f;
    float zoom_speed_ = 1.0f;
    float min_distance_ = 2.0f;
    float max_distance_ = 200.0f;
    float fov_deg_ = 45.0f;
    float near_plane_ = 0.1f;
    float far_plane_ = 200.0f;
};
} // namespace domain
