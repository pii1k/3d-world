#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace domain
{
// A simplified way to identify meshes.
// In a real engine, this would be a handle or ID.
enum class MeshType
{
    Cube,
    // Plane,
};

struct RenderObject
{
    MeshType mesh_type;
    glm::mat4 transform;
    glm::vec3 color;
};

struct SceneData
{
    glm::mat4 view;
    glm::mat4 proj;
    std::vector<RenderObject> objects;
    bool show_ui = false; // For UI panel
};

} // namespace domain