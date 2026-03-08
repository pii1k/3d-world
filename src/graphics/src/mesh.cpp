#include "mesh.hpp"

namespace graphics
{
Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices)
    : vertices_(vertices),
      indices_(indices)
{
}
} // namespace graphics