#include "mesh.hpp"

#include <cstddef>

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    : vertices_(vertices),
      indices_(indices)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

    constexpr GLsizei stride = sizeof(Vertex);
    glEnableVertexAttribArray(0); // 위치
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1); // 법선
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2); // 텍스처 좌표
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offsetof(Vertex, texture_coordinates)));

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (ebo_ != 0)
        glDeleteBuffers(1, &ebo_);
    if (vbo_ != 0)
        glDeleteBuffers(1, &vbo_);
    if (vao_ != 0)
        glDeleteVertexArrays(1, &vao_);
}