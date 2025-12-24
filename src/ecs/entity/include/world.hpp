#pragma once

#include <cstdint>
#include <functional>
#include <glm/fwd.hpp>
#include <memory>
#include <optional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "component.hpp"
#include "component_array.hpp"

using entity_id = std::uint32_t;

class World
{
public:
    using Entity = entity_id;

    [[nodiscard]]
    Entity newEntity()
    {
        if (!available_entities_.empty())
        {
            Entity entity = available_entities_.back();
            available_entities_.pop_back();
            return entity;
        }
        return next_entity_id_++;
    }

    void destroyEntity(Entity entity)
    {
        for (auto &entry : component_pools_)
        {
            if (entry.second)
                entry.second->remove(entity);
        }
        available_entities_.push_back(entity);
    }

    template <typename T>
    std::decay_t<T> &addComponent(Entity entity, T &&component)
    {
        using Component = std::decay_t<T>;
        auto *array = getOrCreateArray<Component>();
        return array->insertData(entity, std::forward<T>(component));
    }

    template <typename T>
    bool removeComponent(Entity entity)
    {
        auto *array = getArray<std::decay_t<T>>();
        if (!array || !array->contains(entity))
            return false;
        array->removeData(entity);
        return true;
    }

    template <typename T>
    [[nodiscard]]
    std::optional<std::reference_wrapper<std::decay_t<T>>> getComponent(Entity entity)
    {
        auto *array = getArray<std::decay_t<T>>();
        return array ? array->find(entity) : std::nullopt;
    }

    template <typename T>
    [[nodiscard]]
    std::optional<std::reference_wrapper<const std::decay_t<T>>> getComponent(Entity entity) const
    {
        const auto *array = getArray<std::decay_t<T>>();
        return array ? array->find(entity) : std::nullopt;
    }

    TransformComponent &addTransform(Entity entity, TransformComponent component)
    {
        return addComponent(entity, std::move(component));
    }

    RenderableComponent &addRenderable(Entity entity, RenderableComponent component)
    {
        return addComponent(entity, std::move(component));
    }

    LightComponent &addLight(Entity entity, LightComponent component)
    {
        return addComponent(entity, std::move(component));
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

    template <typename T, typename Func>
    void forEachComponent(Func &&func)
    {
        auto *array = getArray<std::decay_t<T>>();
        if (!array)
            return;
        for (const auto &[entity, _] : array->entities())
        {
            func(entity, array->getData(entity));
        }
    }

    template <typename T, typename Func>
    void forEachComponent(Func &&func) const
    {
        const auto *array = getArray<std::decay_t<T>>();
        if (!array)
            return;
        for (const auto &[entity, _] : array->entities())
        {
            func(entity, array->getData(entity));
        }
    }

    Entity newPlayer(const glm::vec3 &position)
    {
        Entity entity = newEntity();
        TransformComponent transform{};
        transform.position = position;
        addTransform(entity, std::move(transform));
        return entity;
    }

private:
    template <typename T>
    ComponentArray<T> *getArray()
    {
        const auto type_idx = std::type_index(typeid(T));
        auto it = component_pools_.find(type_idx);
        if (it == component_pools_.end())
            return nullptr;
        return static_cast<ComponentArray<T> *>(it->second.get());
    }

    template <typename T>
    const ComponentArray<T> *getArray() const
    {
        const auto type_idx = std::type_index(typeid(T));
        auto it = component_pools_.find(type_idx);
        if (it == component_pools_.end())
            return nullptr;
        return static_cast<const ComponentArray<T> *>(it->second.get());
    }

    template <typename T>
    ComponentArray<T> *getOrCreateArray()
    {
        const auto type_idx = std::type_index(typeid(T));
        auto it = component_pools_.find(type_idx);
        if (it == component_pools_.end())
        {
            auto [inserted_it, _] = component_pools_.emplace(type_idx, std::make_unique<ComponentArray<T>>());
            return static_cast<ComponentArray<T> *>(inserted_it->second.get());
        }
        return static_cast<ComponentArray<T> *>(it->second.get());
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<Interface::ComponentArray>> component_pools_;
    std::vector<Entity> available_entities_;
    Entity next_entity_id_ = 0;
};