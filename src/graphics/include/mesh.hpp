#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "shader.hpp"

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

    void draw(const Shader &shader); //
};