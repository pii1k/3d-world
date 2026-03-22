#pragma once

#include "bat_types.hpp"

namespace domain
{
class BatInput
{
public:
    void onKeyEvent(int key, int action);
    void onCursorMove(double delta_x, double delta_y);
    InputState currentState() const { return state_; }

private:
    InputState state_{};
    float look_sensitivity_ = 0.12f;
    float min_pitch_deg_ = -65.0f;
    float max_pitch_deg_ = 35.0f;
};
} // namespace domain