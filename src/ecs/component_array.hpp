#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

// Component들을 연속 메모리에 보관하기 위한 컨테이너.
// Registry가 내부적으로 사용하며, 특정 타입의 컴포넌트를 엔티티 ID와 매핑한다.
using Entity = std::uint32_t;

namespace Interface
{
class ComponentArray
{
public:
    virtual ~ComponentArray() = default;
};
} // namespace Interface

template <typename T>
class ComponentArray : public Interface::ComponentArray
{
public:
    template <typename... Args>
    T &insertData(Entity entity, Args &&...args)
    {
        if (auto it = entity_to_index_map.find(entity); it != entity_to_index_map.end())
        {
            component_data_[it->second] = T{std::forward<Args>(args)...};
            return component_data_[it->second];
        }

        size_t new_index = component_data_.size();
        entity_to_index_map[entity] = new_index;
        index_to_entity_map[new_index] = entity;
        component_data_.emplace_back(std::forward<Args>(args)...);
        return component_data_.back();
    }

    [[nodiscard]]
    T &getData(Entity entity)
    {
        size_t index = entity_to_index_map.at(entity);
        return component_data_[index];
    }

    [[nodiscard]]
    const T &getData(Entity entity) const
    {
        size_t index = entity_to_index_map.at(entity);
        return component_data_[index];
    }

    [[nodiscard]]
    const T *tryGetData(Entity entity) const
    {
        auto it = entity_to_index_map.find(entity);
        if (it == entity_to_index_map.end())
            return nullptr;
        return &component_data_[it->second];
    }

    [[nodiscard]]
    T *tryGetData(Entity entity)
    {
        auto it = entity_to_index_map.find(entity);
        if (it == entity_to_index_map.end())
            return nullptr;
        return &component_data_[it->second];
    }

    [[nodiscard]]
    const std::unordered_map<Entity, size_t> &entities() const noexcept
    {
        return entity_to_index_map;
    }

    [[nodiscard]]
    const std::vector<T> &raw() const noexcept
    {
        return component_data_;
    }

    [[nodiscard]]
    bool contains(Entity entity) const noexcept
    {
        return entity_to_index_map.find(entity) != entity_to_index_map.end();
    }

    [[nodiscard]]
    std::optional<std::reference_wrapper<T>> find(Entity entity) noexcept
    {
        if (auto it = entity_to_index_map.find(entity); it != entity_to_index_map.end())
            return std::ref(component_data_[it->second]);
        return std::nullopt;
    }

    [[nodiscard]]
    std::optional<std::reference_wrapper<const T>> find(Entity entity) const noexcept
    {
        if (auto it = entity_to_index_map.find(entity); it != entity_to_index_map.end())
            return std::cref(component_data_[it->second]);
        return std::nullopt;
    }

    void removeData(Entity entity)
    {
        auto it = entity_to_index_map.find(entity);
        if (it == entity_to_index_map.end())
            return;

        size_t index_of_removed = it->second;
        size_t index_of_last = component_data_.size() - 1;

        component_data_[index_of_removed] = std::move(component_data_[index_of_last]);
        Entity entity_of_last = index_to_entity_map[index_of_last];
        entity_to_index_map[entity_of_last] = index_of_removed;
        index_to_entity_map[index_of_removed] = entity_of_last;

        component_data_.pop_back();
        entity_to_index_map.erase(entity);
        index_to_entity_map.erase(index_of_last);
    }

private:
    std::vector<T> component_data_;
    std::unordered_map<Entity, size_t> entity_to_index_map;
    std::unordered_map<size_t, Entity> index_to_entity_map;
};