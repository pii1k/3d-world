#pragma once

#include "camera.hpp"

#include <memory>

namespace controller
{
class CameraController
{
public:
    explicit CameraController(const CameraConfig &cam_cfg);

    Camera &getCamera()
    {
        return *camera_;
    }
    const Camera &getCamera() const
    {
        return *camera_;
    }

    void update();
    void onScroll(double yoffset);
    void onResize(int width, int height);

private:
    std::unique_ptr<Camera> camera_;

    glm::vec3 target_{0.0f, 0.0f, 0.0f};
    float yaw_deg_ = 0.0f;
    float pitch_deg_ = 35.0f;
    float distance_ = 50.0f;
    float zoom_speed_ = 1.0f;
    float min_distance_ = 2.0f;
    float max_distance_ = 200.0f;
};
} // namespace controller