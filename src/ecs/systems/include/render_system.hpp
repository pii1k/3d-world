#pragma once

#include "component.hpp"
#include "render_data.hpp"
#include "world.hpp"
#include <utility>

class RenderSystem
{
public:
    void update(const World &world, RenderQueue &queue) const
    {
        queue.clear();

        world.forEachComponent<RenderableComponent>(
            [&](World::Entity entity, const RenderableComponent &renderable)
            {
                auto transform = world.getComponent<TransformComponent>(entity);
                if (!transform)
                    return;

                RenderItem item{};
                item.mesh_handle = static_cast<MeshHandle>(renderable.mesh_id);
                item.material_handle = 0; // TODO(jyan): hook up material component/pipeline
                item.model = transform->get().getTransform();
                item.color = renderable.color;
                item.use_grid = renderable.use_grid;
                item.pass = RenderPass::Opaque;

                // opaque for now; if transparent flag added, compute distance and call addTransparent
                queue.addOpaque(std::move(item));
            });

        queue.sort();
    }
};
