#pragma once

#include <OpenGL/OpenGL.h>
#include <glm/glm.hpp>
#include <vector>

namespace graphics
{
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices,
         const std::vector<uint32_t> &indices);
    ~Mesh();

    GLuint getVAO() const { return vao_; }
    size_t getIndexCount() const { return indices_.size(); }

private:
    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    GLuint vao_, vbo_, ebo_ = 0;
};

} // namespace graphics