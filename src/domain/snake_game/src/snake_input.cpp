#include "snake_input.hpp"
#include "snake_logic.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace domain
{
void SnakeInput::onKeyPressed(int key, SnakeLogic &logic, bool &show_ui)
{
    if (key == GLFW_KEY_P)
    {
        logic.togglePause();
        return;
    }

    if (key == GLFW_KEY_R)
    {
        logic.reset();
        return;
    }

    if (logic.state().game_over || logic.state().paused)
        return;

    switch (key)
    {
    case GLFW_KEY_UP:
    case GLFW_KEY_W:
        logic.setPendingDir({0, -1});
        break;
    case GLFW_KEY_DOWN:
    case GLFW_KEY_S:
        logic.setPendingDir({0, 1});
        break;
    case GLFW_KEY_LEFT:
    case GLFW_KEY_A:
        logic.setPendingDir({-1, 0});
        break;
    case GLFW_KEY_RIGHT:
    case GLFW_KEY_D:
        logic.setPendingDir({1, 0});
        break;
    default:
        break;
    }
}
} // namespace domain
