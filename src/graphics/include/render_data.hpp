#pragma once

#include <algorithm>
#include <cstdint>
#include <glm/glm.hpp>
#include <utility>
#include <vector>

using Matrix4x4 = glm::mat4;
using MeshHandle = uint32_t;
using MaterialHandle = uint32_t;

enum class MeshId : int
{
    Cube = 0,
    Plane = 1,
};

inline uint64_t makeOpaqueKey(MaterialHandle material_handle, MeshHandle mesh_handle)
{
    uint64_t key = 0;
    key |= (uint64_t)0 << 63;
    key |= (uint64_t)material_handle << 32;
    key |= (uint64_t)mesh_handle << 0;
    return key;
}

inline uint64_t makeTransparentKey(float distance_to_camera)
{
    uint32_t distance = (uint32_t)std::min(distance_to_camera * 1000.0f, 4'000'000'000.0f);
    uint64_t key = 0;
    key |= (uint64_t)1 << 63;
    key |= (uint64_t)(0xFFFFFFFFu - distance);
    return key;
}

enum class RenderPass : uint8_t
{
    Opaque = 0,
    Transparent = 1,
};

struct RenderItem
{
    MeshHandle mesh_handle{};
    MaterialHandle material_handle{};
    Matrix4x4 model{1.0f};
    glm::vec3 color{1.0f};
    bool use_grid{};
    RenderPass pass{RenderPass::Opaque};
    uint64_t sort_key{0};
};

struct RenderQueue
{
    std::vector<RenderItem> opaque;
    std::vector<RenderItem> transparent;

    void clear()
    {
        opaque.clear();
        transparent.clear();
    }

    void reserve(std::size_t opaque_count, std::size_t transparent_count = 0)
    {
        opaque.reserve(opaque_count);
        transparent.reserve(transparent_count);
    }

    void addOpaque(RenderItem item)
    {
        item.pass = RenderPass::Opaque;
        item.sort_key = makeOpaqueKey(item.material_handle, item.mesh_handle);
        opaque.push_back(std::move(item));
    }

    void addTransparent(RenderItem item)
    {
        item.pass = RenderPass::Transparent;
        transparent.push_back(std::move(item));
    }

    void sort()
    {
        auto by_key = [](const RenderItem &lhs, const RenderItem &rhs) { return lhs.sort_key < rhs.sort_key; };
        std::sort(opaque.begin(), opaque.end(), by_key);
        std::sort(transparent.begin(), transparent.end(), by_key); // far -> near due to key encoding
    }
};
