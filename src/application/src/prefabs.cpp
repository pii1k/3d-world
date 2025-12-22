#include "prefabs.hpp"

#include <glm/glm.hpp>

namespace Prefabs
{
Entity createPlayer(Registry &registry, int mesh_id)
{
    Entity entity = registry.newEntity();
    TransformComponent transform{};
    transform.position = {0.0f, 0.5f, 0.0f};
    registry.addTransform(entity, transform);

    RenderableComponent renderable{};
    renderable.mesh_id = mesh_id;
    renderable.color = {1.0f, 0.5f, 0.2f}; // 주황 큐브
    registry.addRenderable(entity, renderable);
    return entity;
}

Entity createGround(Registry &registry, int mesh_id, float size)
{
    Entity entity = registry.newEntity();

    TransformComponent transform{};
    transform.scale = glm::vec3(size, 1.0f, size); // 평면을 원하는 크기로 확장
    registry.addTransform(entity, transform);

    RenderableComponent renderable{};
    renderable.mesh_id = mesh_id;
    renderable.color = {0.22f, 0.22f, 0.24f};
    renderable.use_grid = true; // 그리드 패턴 표시
    registry.addRenderable(entity, renderable);

    return entity;
}
} // namespace Prefabs