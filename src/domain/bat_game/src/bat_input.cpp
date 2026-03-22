#include "bat_input.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

namespace domain
{
void BatInput::onKeyEvent(int key, int action)
{
    const bool pressed = (action == GLFW_PRESS || action == GLFW_REPEAT);
    const bool released = (action == GLFW_RELEASE);
    if (!pressed && !released)
        return;

    switch (key)
    {
    case GLFW_KEY_W:
        state_.w = pressed;
        break;
    case GLFW_KEY_A:
        state_.a = pressed;
        break;
    case GLFW_KEY_S:
        state_.s = pressed;
        break;
    case GLFW_KEY_D:
        state_.d = pressed;
        break;
    default:
        break;
    }
}

void BatInput::onCursorMove(double delta_x, double delta_y)
{
    state_.look_yaw_deg -= static_cast<float>(delta_x) * look_sensitivity_;
    state_.look_pitch_deg = glm::clamp(state_.look_pitch_deg - static_cast<float>(delta_y) * look_sensitivity_,
                                       min_pitch_deg_,
                                       max_pitch_deg_);
}
} // namespace domain