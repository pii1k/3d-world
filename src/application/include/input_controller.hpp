#pragma once

#include <glm/glm.hpp>

class InputController
{
public:
    InputController();

    void onKey(int key, int action);
    void onScroll(double yoffset);
    void onMouseClick(double cursor_x,
                      double cursor_y,
                      int viewport_w,
                      int viewport_h,
                      const glm::mat4 &view,
                      const glm::mat4 &proj);

    void update(float dt_s);

private:
    static bool rayIntersectsAABB(const glm::vec3 &ray_origin,
                                  const glm::vec3 &ray_dir,
                                  const glm::vec3 &aabb_min,
                                  const glm::vec3 &aabb_max);

    glm::vec3 &cube_pos_;
    bool &show_ui_;
    float &move_speed_;

    struct
    {
        bool w = false;
        bool a = false;
        bool s = false;
        bool d = false;
    } input_;
};