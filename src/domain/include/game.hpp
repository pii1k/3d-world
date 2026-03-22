#pragma once

#include "event.hpp"
#include "render_defs.hpp"
#include <memory>

namespace domain
{
class Game
{
public:
    virtual ~Game() = default;

    virtual std::unique_ptr<Game> createGame() = 0;

    virtual void init() = 0;
    virtual void update(float dt_s) = 0;
    virtual void onEvent(const Event &e) = 0;
    virtual void setScene(SceneData &out_scene) = 0;
};
} // namespace domain