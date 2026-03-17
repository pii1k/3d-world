#pragma once

#include "bat_types.hpp"

namespace domain
{
class BatInput
{
public:
    void onKeyEvent(int key, int action);
    InputState currentState() const { return state_; }

private:
    InputState state_{};
};
} // namespace domain
