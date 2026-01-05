#include "input_controller.hpp"
#include "camera.hpp"
#include "camera_system.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <limits>

void InputController::onMouseMove(double xpos, double ypos)
{
    if (first_mouse_)
    {
        last_x_ = xpos;
        last_y_ = ypos;
        first_mouse_ = false;
        return;
    }

    const double d_x = xpos - last_x_;
    const double d_y = last_y_ - ypos;
    last_x_ = xpos;
    last_y_ = ypos;

    mouse_dx_ += static_cast<float>(d_x);
    mouse_dy_ += static_cast<float>(d_y);
}

void InputController::onKey(int key, int action)
{
    const bool pressed = (action == GLFW_PRESS);
    const bool released = (action == GLFW_RELEASE);
    if (!pressed && !released)
        return;

    switch (key)
    {
    case GLFW_KEY_W:
        input_.w = pressed;
        break;
    case GLFW_KEY_A:
        input_.a = pressed;
        break;
    case GLFW_KEY_S:
        input_.s = pressed;
        break;
    case GLFW_KEY_D:
        input_.d = pressed;
        break;
    default:
        break;
    }
}

void InputController::onScroll(double yoffset)
{
    scroll_y_ += static_cast<float>(yoffset);
}

void InputController::cameraUpdate(float delta_time, CameraSystem &camera_system, Camera &camera)
{
    camera_system.update(mouse_dx_,
                         mouse_dy_,
                         scroll_y_,
                         delta_time,
                         camera);
    mouse_dx_ = 0.0f;
    mouse_dy_ = 0.0f;
    scroll_y_ = 0.0f;
}

bool InputController::onMouseClick(double cursor_x,
                                   double cursor_y,
                                   int viewport_w,
                                   int viewport_h,
                                   const glm::mat4 &view,
                                   const glm::mat4 &proj,
                                   const glm::vec3 &target_position,
                                   const glm::vec3 &half_extents)
{
    const float x_ndc = static_cast<float>((2.0 * cursor_x) / std::max(1, viewport_w) - 1.0);
    const float y_ndc = static_cast<float>(1.0 - (2.0 * cursor_y) / std::max(1, viewport_h));

    const glm::mat4 inv_vp = glm::inverse(proj * view);
    const glm::vec4 near_clip{x_ndc, y_ndc, -1.0f, 1.0f};
    const glm::vec4 far_clip{x_ndc, y_ndc, 1.0f, 1.0f};
    glm::vec4 near_world = inv_vp * near_clip;
    glm::vec4 far_world = inv_vp * far_clip;
    near_world /= std::max(1e-6f, near_world.w);
    far_world /= std::max(1e-6f, far_world.w);

    const glm::vec3 ray_origin{near_world};
    const glm::vec3 ray_dir = glm::normalize(glm::vec3(far_world - near_world));

    const glm::vec3 aabb_min = target_position - half_extents;
    const glm::vec3 aabb_max = target_position + half_extents;
    return rayIntersectsAABB(ray_origin, ray_dir, aabb_min, aabb_max);
}

bool InputController::rayIntersectsAABB(const glm::vec3 &ray_origin,
                                        const glm::vec3 &ray_dir,
                                        const glm::vec3 &aabb_min,
                                        const glm::vec3 &aabb_max)
{
    float tmin = -std::numeric_limits<float>::infinity();
    float tmax = std::numeric_limits<float>::infinity();

    for (int axis = 0; axis < 3; ++axis)
    {
        const float origin = ray_origin[axis];
        const float dir = ray_dir[axis];
        const float minv = aabb_min[axis];
        const float maxv = aabb_max[axis];

        if (std::abs(dir) < 1e-6f)
        {
            if (origin < minv || origin > maxv)
                return false;
            continue;
        }

        const float inv_dir = 1.0f / dir;
        float t1 = (minv - origin) * inv_dir;
        float t2 = (maxv - origin) * inv_dir;
        if (t1 > t2)
            std::swap(t1, t2);

        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmax < tmin)
            return false;
    }

    return tmax >= 0.0f;
}