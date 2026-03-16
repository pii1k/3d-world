#include "snake_logic.hpp"

#include <algorithm>
#include <vector>

namespace domain
{
SnakeLogic::SnakeLogic()
{
    std::random_device rd;
    rng_ = std::mt19937(rd());
    this->init();
}

void SnakeLogic::init()
{
    const int start_x = grid_.width / 2;
    const int start_y = grid_.height / 2;
    snake_.push_back({start_x, start_y});
    snake_.push_back({start_x - 1, start_y});
    snake_.push_back({start_x - 2, start_y});
    direction_ = {1, 0};
    next_direction_ = direction_;
}

void SnakeLogic::reset()
{
    state_.paused = false;
    state_.game_over = false;
    state_.score = 0;
    tick_accum_s_ = 0.0f;
    snake_.clear();

    this->init();
    this->spawnFood();
}

void SnakeLogic::update(float dt_s)
{
    tick_accum_s_ += dt_s;
    tick_accum_s_ = std::min(tick_accum_s_, 1.0f);

    if (state_.paused || state_.game_over)
        return;

    while (tick_accum_s_ >= tick_interval_s_)
    {
        step();
        tick_accum_s_ -= tick_interval_s_;
        if (state_.game_over)
            break;
    }
}

void SnakeLogic::togglePause()
{
    state_.paused = !state_.paused;
}

void SnakeLogic::setPendingDir(const glm::ivec2 &dir)
{
    if (dir.x == 0 && dir.y == 0)
        return;

    if (snake_.size() > 1)
    {
        if ((dir.x + this->direction_.x) == 0 && (dir.y + this->direction_.y) == 0)
            return;
    }

    next_direction_ = dir;
}

void SnakeLogic::step()
{
    if (snake_.empty())
        return;

    direction_ = next_direction_;
    const glm::ivec2 new_head{snake_.front().x + direction_.x, snake_.front().y + direction_.y};

    if (new_head.x < 0 || new_head.x >= grid_.width ||
        new_head.y < 0 || new_head.y >= grid_.height)
    {
        state_.game_over = true;
        return;
    }

    const bool will_grow = (new_head.x == food_.x && new_head.y == food_.y);
    const size_t body_check_count = will_grow ? snake_.size() : (snake_.empty() ? 0 : snake_.size() - 1);
    for (size_t i = 0; i < body_check_count; ++i)
    {
        if (snake_[i].x == new_head.x && snake_[i].y == new_head.y)
        {
            state_.game_over = true;
            return;
        }
    }

    snake_.push_front(new_head);

    if (will_grow)
    {
        state_.score += 1;
        spawnFood();
    }
    else
    {
        snake_.pop_back();
    }
}

void SnakeLogic::spawnFood()
{
    std::vector<glm::ivec2> empty_cells;
    empty_cells.reserve(static_cast<size_t>(grid_.width * grid_.height) - snake_.size());

    for (int y = 0; y < grid_.height; ++y)
    {
        for (int x = 0; x < grid_.width; ++x)
        {
            const glm::ivec2 cell{x, y};
            if (!isOccupied(cell))
                empty_cells.push_back(cell);
        }
    }

    if (empty_cells.empty())
    {
        state_.game_over = true;
        return;
    }

    size_t max_idx = empty_cells.size() - 1;
    std::uniform_int_distribution<size_t> dist(0, max_idx);
    food_ = empty_cells[dist(rng_)];
}

bool SnakeLogic::isOccupied(const glm::ivec2 &cell) const
{
    for (const auto &segment : snake_)
    {
        if (segment.x == cell.x && segment.y == cell.y)
            return true;
    }
    return false;
}
} // namespace domain