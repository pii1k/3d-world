// 현재는 Registry를 순회하며 렌더링이 필요한 엔티티를 찾아, 그리기 명령(DrawCommand)을 생성하는 역할만 함

#pragma once

#include "component.hpp"
#include "registry.hpp"
#include "render_data.hpp"

class RenderSystem
{
public:
    void update(Registry &registry, RenderQueue &queue)
    {
        registry.forEachRenderable([&queue](Entity /*entity*/,
                                            const TransformComponent &transform,
                                            const RenderableComponent &renderable)
                                   { queue.push_back({renderable.mesh_id,
                                                      transform.getTransform(),
                                                      renderable.color,
                                                      renderable.use_grid}); });
    }
};