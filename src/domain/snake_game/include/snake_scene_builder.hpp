#pragma once

#include "render_defs.hpp"

#include <glm/glm.hpp>

namespace domain
{
class SnakeLogic;

class SnakeSceneBuilder
{
public:
    void buildScene(const SnakeLogic &logic, SceneData &out_scene) const;

private:
    glm::vec3 gridToWorld(const SnakeLogic &logic, const glm::ivec2 &cell) const;
};
} // namespace domain