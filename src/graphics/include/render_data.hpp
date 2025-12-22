#pragma once

#include <glm/glm.hpp>
#include <vector>

using Matrix4x4 = glm::mat4;

enum class MeshId : int
{
    Cube = 0,
    Plane = 1,
};

struct DrawCommand
{
    int mesh_id;
    Matrix4x4 transform;
    glm::vec3 color;
    bool use_grid;
};

using RenderQueue = std::vector<DrawCommand>;