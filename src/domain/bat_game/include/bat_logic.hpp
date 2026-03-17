#pragma once

#include "bat_types.hpp"

namespace domain
{
class BatLogic
{
public:
    void init();
    void update(float dt_s, const InputState &input);

    const BatState &state() const { return state_; }
    const BatStats &stats() const { return stats_; }

private:
    BatState state_{};
    BatStats stats_{};
    float hover_time_s_ = 0.0f;
};
} // namespace domain
