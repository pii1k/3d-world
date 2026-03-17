#pragma once

#include "bat_logic.hpp"
#include "render_defs.hpp"

namespace domain
{
class BatSceneBuilder
{
public:
    void buildScene(const BatLogic &logic, SceneData &out_scene) const;
};
} // namespace domain
