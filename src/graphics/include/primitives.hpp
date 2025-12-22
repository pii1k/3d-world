#pragma once

#include <memory>

#include "mesh.hpp"

namespace Primitives
{
std::unique_ptr<Mesh> createCube();
std::unique_ptr<Mesh> createPlane(float width, float height);
} // namespace Primitives