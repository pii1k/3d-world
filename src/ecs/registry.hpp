// ECS의 핵심 허브. 엔티티를 생성하고, 엔티티에 컴포넌트를 붙이고, 특정 컴포넌트들을 가진 엔티티를 찾는 역할을 한다. 처음이라 아주 간단하게 시작
#pragma once

#include "component.hpp"
#include <cstdint>
#include <unordered_map>

// entity는 고유한 id일 뿐이다.
using Entity = std::uint32_t;

class Registry
{
public:
    // 새로운 엔티티를 생성하고 고유 id를 반환
    Entity createEntity()
    {
        return next_entity_++;
    };

    // 엔티티에 컴포넌트 추가
    void addTransform(Entity entity, TransformComponent component)
    {
        transforms_[entity] = component;
    }

    void addRenderable(Entity entity, RenderableComponent component)
    {
        renderables_[entity] = component;
    }

    void addLight(Entity entity, LightComponent component)
    {
        lights_[entity] = component;
    }

    TransformComponent *getTransform(Entity entity)
    {
        auto it = transforms_.find(entity);
        return it != transforms_.end() ? &it->second : nullptr;
    }

    const TransformComponent *getTransform(Entity entity) const
    {
        auto it = transforms_.find(entity);
        return it != transforms_.end() ? &it->second : nullptr;
    }

    RenderableComponent *getRenderable(Entity entity)
    {
        auto it = renderables_.find(entity);
        return it != renderables_.end() ? &it->second : nullptr;
    }

    const RenderableComponent *getRenderable(Entity entity) const
    {
        auto it = renderables_.find(entity);
        return it != renderables_.end() ? &it->second : nullptr;
    }

    template <typename Func>
    void forEachRenderable(Func &&func)
    {
        for (auto &pair : transforms_)
        {
            auto renderable_it = renderables_.find(pair.first);
            if (renderable_it != renderables_.end())
            {
                func(pair.first, pair.second, renderable_it->second);
            }
        }
    }

    template <typename Func>
    void forEachRenderable(Func &&func) const
    {
        for (const auto &pair : transforms_)
        {
            auto renderable_it = renderables_.find(pair.first);
            if (renderable_it != renderables_.end())
            {
                func(pair.first, pair.second, renderable_it->second);
            }
        }
    }

    const std::unordered_map<Entity, TransformComponent> &getTransforms() const { return transforms_; }
    const std::unordered_map<Entity, RenderableComponent> &getRenderables() const { return renderables_; }

    // (나중에 더 효율적인 방식으로 개선) 지금은 간단하게 map 사용
private:
    std::unordered_map<Entity, TransformComponent> transforms_;
    std::unordered_map<Entity, RenderableComponent> renderables_;
    std::unordered_map<Entity, LightComponent> lights_;

private:
    Entity next_entity_ = 0;
};