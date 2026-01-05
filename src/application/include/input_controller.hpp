#pragma once

#include <glm/glm.hpp>

class Camera;
class CameraSystem;

class InputController
{
public:
    InputController() = default;

    void onMouseMove(double xpos, double ypos);
    void onKey(int key, int action);
    void onScroll(double yoffset);
    bool onMouseClick(double cursor_x,
                      double cursor_y,
                      int viewport_w,
                      int viewport_h,
                      const glm::mat4 &view,
                      const glm::mat4 &proj,
                      const glm::vec3 &target_position,
                      const glm::vec3 &half_extents);

    void cameraUpdate(float delta_time, CameraSystem &camera_system, Camera &camera);

private:
    struct
    {
        bool w = false;
        bool a = false;
        bool s = false;
        bool d = false;
    } input_;

    bool first_mouse_ = true;
    double last_x_ = 0.0;
    double last_y_ = 0.0;
    float mouse_dx_ = 0.0f;
    float mouse_dy_ = 0.0f;
    float scroll_y_ = 0.0f;

    static bool rayIntersectsAABB(const glm::vec3 &ray_origin,
                                  const glm::vec3 &ray_dir,
                                  const glm::vec3 &aabb_min,
                                  const glm::vec3 &aabb_max);
};