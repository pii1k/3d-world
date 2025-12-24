#pragma once

#include "component.hpp"
#include "render_data.hpp"
#include "world.hpp"

class RenderSystem
{
public:
    void update(const World &world, RenderQueue &queue) const
    {
        world.forEachRenderable([&queue](World::Entity /*entity*/,
                                         const TransformComponent &transform,
                                         const RenderableComponent &renderable)
                                { queue.push_back({renderable.mesh_id,
                                                   transform.getTransform(),
                                                   renderable.color,
                                                   renderable.use_grid}); });
    }
};