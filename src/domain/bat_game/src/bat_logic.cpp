#include "bat_logic.hpp"

#include <algorithm>
#include <glm/gtc/constants.hpp>

namespace domain
{
void BatLogic::init()
{
    state_ = BatState{};
    stats_ = BatStats{};
    hover_time_s_ = 0.0f;
}

void BatLogic::update(float dt_s, const InputState &input)
{
    hover_time_s_ += std::max(0.0f, dt_s);

    glm::vec3 move_dir{0.0f};
    if (input.w)
        move_dir.z -= 1.0f;
    if (input.s)
        move_dir.z += 1.0f;
    if (input.a)
        move_dir.x -= 1.0f;
    if (input.d)
        move_dir.x += 1.0f;

    if (glm::dot(move_dir, move_dir) > 0.0f)
    {
        move_dir = glm::normalize(move_dir);
        state_.forward = move_dir;
    }

    state_.velocity = move_dir * stats_.move_speed;
    state_.position += state_.velocity * dt_s;

    // Simple hover (no gravity for first milestone)
    const float bob = std::sin(hover_time_s_ * glm::two_pi<float>() * stats_.hover_freq) * stats_.hover_amp;
    state_.position.y = stats_.hover_base_y + bob;
}
} // namespace domain
