#include "input_controller.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <limits>

InputController::InputController(glm::vec3 &cube_pos,
                                 bool &show_ui,
                                 float &move_speed)
    : cube_pos_(cube_pos),
      show_ui_(show_ui),
      move_speed_(move_speed) {}

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
    camera_controller_.onScroll(yoffset);
}

void InputController::update(float dt_s)
{
    glm::vec3 move_dir{0.0f};
    if (input_.w)
        move_dir.z -= 1.0f;
    if (input_.s)
        move_dir.z += 1.0f;
    if (input_.a)
        move_dir.x -= 1.0f;
    if (input_.d)
        move_dir.x += 1.0f;

    if (glm::dot(move_dir, move_dir) > 0.0f)
    {
        move_dir = glm::normalize(move_dir);
        cube_pos_ += move_dir * (move_speed_ * dt_s);
    }
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

void InputController::onMouseClick(double cursor_x,
                                   double cursor_y,
                                   int viewport_w,
                                   int viewport_h,
                                   const glm::mat4 &view,
                                   const glm::mat4 &proj)
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

    constexpr glm::vec3 half_extents{0.5f, 0.5f, 0.5f};
    const glm::vec3 aabb_min = cube_pos_ - half_extents;
    const glm::vec3 aabb_max = cube_pos_ + half_extents;
    if (rayIntersectsAABB(ray_origin, ray_dir, aabb_min, aabb_max))
        show_ui_ = !show_ui_;
}