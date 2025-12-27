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
        const auto type_idx = std::type_index(typeid(Component));
        auto [it, _] = component_pools_.try_emplace(type_idx, std::make_unique<ComponentArray<Component>>());
        auto *array = static_cast<ComponentArray<Component> *>(it->second.get());
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
    std::optional<std::reference_wrapper<std::decay_t<T>>> getComponent(Entity entity)
    {
        auto *array = getArray<std::decay_t<T>>();
        return array ? array->find(entity) : std::nullopt;
    }

    template <typename T, typename Func>
    void forEachComponent(Func &&func)
    {
        auto *array = getArray<std::decay<T>>();
        if (!array)
            return;

        for (const auto &[entity, _] : array->entities())
        {
            func(entity, array->getData(entity));
        }
    }

    template <typename T>
    std::optional<std::reference_wrapper<const std::decay_t<T>>> getComponent(Entity entity) const
    {
        const auto *array = getArray<std::decay_t<T>>();
        return array ? array->find(entity) : std::nullopt;
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
        addComponent(entity, std::move(transform));
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

private:
    std::unordered_map<std::type_index, std::unique_ptr<Interface::ComponentArray>> component_pools_;
    std::vector<Entity> available_entities_;
    Entity next_entity_id_ = 0;
};