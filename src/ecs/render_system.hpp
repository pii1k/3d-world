// Registry를 순회하며 렌더링이 필요한 엔티티를 찾아, 그리기 명령(DrawCommand)을 생성하는 역할만 함
#pragma once

#include "../graphics/render_data.hpp"
#include "component.hpp"
#include "registry.hpp"

class RenderSystem
{
public:
    void update(Registry &registry, RenderQueue &queue)
    {

        for (const auto &pair : registry.transforms_)
        {
            Entity entity_id = pair.first;
            if (registry.renderables_.count(entity_id))
            {
                const RenderableComponent &renderable = registry.renderables_.at(entity_id);
                const TransformComponent &transform = pair.second;

                queue.push_back({renderable.model_id, transform.getTransform()});
            }
        }
    }
};