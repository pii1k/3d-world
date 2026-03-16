#pragma once

namespace domain
{
class SnakeLogic;

class SnakeInput
{
public:
    void onKeyPressed(int key, SnakeLogic &logic, bool &show_ui);
};
} // namespace domain