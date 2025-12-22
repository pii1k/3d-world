#include "primitives.hpp"

#include <glm/glm.hpp>

namespace Primitives
{
namespace
{
Vertex makeVertex(const glm::vec3 &pos, const glm::vec3 &normal, const glm::vec2 &uv)
{
    return Vertex{pos, normal, uv};
}
} // namespace

std::unique_ptr<Mesh> createCube()
{
    // 각 면마다 고유한 법선을 갖도록 24개의 정점 정의
    const std::vector<Vertex> vertices = {
        // 앞면
        makeVertex({-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),
        makeVertex({0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),
        makeVertex({0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}),
        makeVertex({-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}),

        // 뒷면
        makeVertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}),
        makeVertex({-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}),
        makeVertex({0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}),
        makeVertex({0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}),

        // 왼쪽면
        makeVertex({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        makeVertex({-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        makeVertex({-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        makeVertex({-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),

        // 오른쪽면
        makeVertex({0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        makeVertex({0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        makeVertex({0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        makeVertex({0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),

        // 아래면
        makeVertex({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}),
        makeVertex({0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}),
        makeVertex({0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}),
        makeVertex({-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}),

        // 윗면
        makeVertex({-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}),
        makeVertex({0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}),
        makeVertex({0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}),
        makeVertex({-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}),
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2, 0, 2, 3,       // 앞면
        4, 5, 6, 4, 6, 7,       // 뒷면
        8, 9, 10, 8, 10, 11,    // 왼쪽면
        12, 13, 14, 12, 14, 15, // 오른쪽면
        16, 17, 18, 16, 18, 19, // 아래면
        20, 21, 22, 20, 22, 23  // 윗면
    };

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> createPlane(float width, float height)
{
    const float half_width = width * 0.5f;
    const float half_height = height * 0.5f;

    const std::vector<Vertex> vertices = {
        makeVertex({-half_width, 0.0f, -half_height}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}),
        makeVertex({half_width, 0.0f, -half_height}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}),
        makeVertex({half_width, 0.0f, half_height}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}),
        makeVertex({-half_width, 0.0f, half_height}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}),
    };

    const std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};
    return std::make_unique<Mesh>(vertices, indices);
}
} // namespace Primitives