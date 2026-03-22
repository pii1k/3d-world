#pragma once

#include "bat_types.hpp"
#include "camera.hpp"

#include <memory>

namespace domain
{
class BatCameraRig
{
public:
    void init(int width, int height);
    void update(float dt_s, const BatState &bat);
    void onResize(int width, int height);

    Camera &getCamera() { return *camera_; }
    const Camera &getCamera() const { return *camera_; }

private:
    std::unique_ptr<Camera> camera_;
    glm::vec3 smooth_pos_{0.0f};
    float distance_ = 8.0f;
    float height_ = 2.5f;
    float smooth_rate_ = 8.0f;
    float base_fov_deg_ = 55.0f;
};
} // namespace domain