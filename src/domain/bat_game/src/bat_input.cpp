#include "bat_input.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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
} // namespace domain
