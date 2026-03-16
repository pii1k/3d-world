#pragma once

#include <deque>
#include <glm/glm.hpp>
#include <random>

namespace domain
{
struct SnakeState
{
    bool paused = false;
    bool game_over = false;
    int score = 0;
};

struct SnakeGrid
{
    int width = 24;
    int height = 36;
    float cell_size = 1.0f;
};

class SnakeLogic
{
public:
    SnakeLogic();

    void init();
    void reset();
    void update(float dt_s);

    void togglePause();
    void setPendingDir(const glm::ivec2 &dir);

    const SnakeState &state() const { return state_; }
    const SnakeGrid &grid() const { return grid_; }
    const std::deque<glm::ivec2> &snake() const { return snake_; }
    const glm::ivec2 &food() const { return food_; }

private:
    void step();
    void spawnFood();
    bool isOccupied(const glm::ivec2 &cell) const;

    SnakeState state_{};
    SnakeGrid grid_{};

    float tick_interval_s_ = 0.18f;
    float tick_accum_s_ = 0.0f;

    std::deque<glm::ivec2> snake_;
    glm::ivec2 direction_{1, 0};
    glm::ivec2 next_direction_{1, 0};
    glm::ivec2 food_{0, 0};

    std::mt19937 rng_;
};
} // namespace domain
