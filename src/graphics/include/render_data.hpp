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

struct RenderQueue
{
    std::vector<DrawCommand> draw_commands;

    void clear() { draw_commands.clear(); }
    void reserve(size_t n) { draw_commands.reserve(n); };
    void push_back(DrawCommand command) { draw_commands.push_back(std::move(command)); }

    auto begin() noexcept { return draw_commands.begin(); }
    auto end() noexcept { return draw_commands.end(); }
    auto begin() const noexcept { return draw_commands.begin(); }
    auto end() const noexcept { return draw_commands.end(); }
};