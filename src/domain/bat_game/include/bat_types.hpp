#pragma once

#include <glm/glm.hpp>

namespace domain
{
struct BatStats
{
    float move_speed = 6.0f;
    float hover_base_y = 2.0f;
    float hover_amp = 0.25f;
    float hover_freq = 1.4f;
};

struct BatState
{
    glm::vec3 position{0.0f, 2.0f, 0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
};

struct InputState
{
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
};
} // namespace domain
