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

    // (나중에 더 효율적인 방식으로 개선) 지금은 간단하게 map 사용
    std::unordered_map<Entity, TransformComponent> transforms_;
    std::unordered_map<Entity, RenderableComponent> renderables_;

private:
    Entity next_entity_ = 0;
};