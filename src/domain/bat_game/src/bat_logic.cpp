#include "bat_logic.hpp"

#include <algorithm>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

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
    state_.camera_yaw_deg = input.look_yaw_deg;
    state_.camera_pitch_deg = input.look_pitch_deg;

    const float yaw = glm::radians(state_.camera_yaw_deg);
    const glm::vec3 forward_flat{glm::sin(yaw), 0.0f, glm::cos(yaw)};
    const glm::vec3 right_flat{-forward_flat.z, 0.0f, forward_flat.x};

    glm::vec3 move_dir{0.0f};
    if (input.w)
        move_dir += forward_flat;
    if (input.s)
        move_dir -= forward_flat;
    if (input.a)
        move_dir -= right_flat;
    if (input.d)
        move_dir += right_flat;

    if (glm::dot(move_dir, move_dir) > 0.0f)
        move_dir = glm::normalize(move_dir);

    state_.velocity = move_dir * stats_.move_speed;
    state_.position += state_.velocity * dt_s;
    state_.forward = glm::normalize(glm::vec3(forward_flat.x, 0.0f, forward_flat.z));

    // Simple hover (no gravity for first milestone)
    const float bob = std::sin(hover_time_s_ * glm::two_pi<float>() * stats_.hover_freq) * stats_.hover_amp;
    state_.position.y = stats_.hover_base_y + bob;
}
} // namespace domain
