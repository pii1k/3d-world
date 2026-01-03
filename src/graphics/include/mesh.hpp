#pragma once

#include "gl_includes.hpp"
#include <cstddef>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
    ~Mesh();

    GLuint getVAO() const { return vao_; }
    size_t getIndexCount() const { return indices_.size(); }

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    GLuint vao_, vbo_, ebo_ = 0;
};