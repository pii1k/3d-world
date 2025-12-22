// ECS의 핵심 허브. 엔티티를 생성하고, 엔티티에 컴포넌트를 붙이고, 특정 컴포넌트들을 가진 엔티티를 찾는다.
#pragma once

#include "component.hpp"
#include "component_array.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

// entity는 고유한 id일 뿐이다.
using Entity = std::uint32_t;

class Registry
{
public:
    [[nodiscard]] Entity newEntity() noexcept { return next_entity_++; }

    // 일반적인 컴포넌트 추가 (배열이 없으면 생성)
    template <typename T>
    std::decay_t<T> &addComponent(Entity entity, T &&component)
    {
        using Component = std::decay_t<T>;
        auto *array = getOrCreateArray<Component>();
        return array->insertData(entity, std::forward<T>(component));
    }

    // 타입별 단축 함수 (기존 사용성 유지)
    TransformComponent &addTransform(Entity entity, TransformComponent component)
    {
        return addComponent(entity, std::move(component));
    }
    RenderableComponent &addRenderable(Entity entity, RenderableComponent component)
    {
        return addComponent(entity, std::move(component));
    }
    LightComponent &addLight(Entity entity, LightComponent component) { return addComponent(entity, std::move(component)); }

    // 컴포넌트 조회
    template <typename T>
    [[nodiscard]] std::optional<std::reference_wrapper<std::decay_t<T>>> getComponent(Entity entity)
    {
        auto *array = getArray<std::decay_t<T>>();
        return array ? array->find(entity) : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] std::optional<std::reference_wrapper<const std::decay_t<T>>> getComponent(Entity entity) const
    {
        auto *array = getArray<std::decay_t<T>>();
        return array ? array->find(entity) : std::nullopt;
    }

    std::optional<std::reference_wrapper<TransformComponent>> getTransform(Entity entity)
    {
        return getComponent<TransformComponent>(entity);
    }
    std::optional<std::reference_wrapper<const TransformComponent>> getTransform(Entity entity) const
    {
        return getComponent<TransformComponent>(entity);
    }

    std::optional<std::reference_wrapper<RenderableComponent>> getRenderable(Entity entity)
    {
        return getComponent<RenderableComponent>(entity);
    }
    std::optional<std::reference_wrapper<const RenderableComponent>> getRenderable(Entity entity) const
    {
        return getComponent<RenderableComponent>(entity);
    }

    // 특정 컴포넌트 조합을 가진 엔티티 순회 (transform + renderable)
    template <typename Func>
    void forEachRenderable(Func &&func)
    {
        auto *transforms = getArray<TransformComponent>();
        auto *renderables = getArray<RenderableComponent>();
        if (!transforms || !renderables)
            return;

        for (const auto &[entity, _] : renderables->entities())
        {
            if (auto transform = transforms->find(entity))
            {
                func(entity, transform->get(), renderables->getData(entity));
            }
        }
    }

    template <typename Func>
    void forEachRenderable(Func &&func) const
    {
        const auto *transforms = getArray<TransformComponent>();
        const auto *renderables = getArray<RenderableComponent>();
        if (!transforms || !renderables)
            return;

        for (const auto &[entity, _] : renderables->entities())
        {
            if (auto transform = transforms->find(entity))
            {
                func(entity, transform->get(), renderables->getData(entity));
            }
        }
    }

private:
    template <typename T>
    ComponentArray<T> *getArray()
    {
        const auto type_idx = std::type_index(typeid(T));
        auto it = component_arrays_.find(type_idx);
        if (it == component_arrays_.end())
            return nullptr;
        return static_cast<ComponentArray<T> *>(it->second.get());
    }

    template <typename T>
    const ComponentArray<T> *getArray() const
    {
        const auto type_idx = std::type_index(typeid(T));
        auto it = component_arrays_.find(type_idx);
        if (it == component_arrays_.end())
            return nullptr;
        return static_cast<const ComponentArray<T> *>(it->second.get());
    }

    template <typename T>
    ComponentArray<T> *getOrCreateArray()
    {
        const auto type_idx = std::type_index(typeid(T));
        auto it = component_arrays_.find(type_idx);
        if (it == component_arrays_.end())
        {
            auto [inserted_it, _] = component_arrays_.emplace(type_idx, std::make_unique<ComponentArray<T>>());
            return static_cast<ComponentArray<T> *>(inserted_it->second.get());
        }
        return static_cast<ComponentArray<T> *>(it->second.get());
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<Interface::ComponentArray>> component_arrays_;
    Entity next_entity_ = 0;
};