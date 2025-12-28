#include "prefabs.hpp"
#include "component.hpp"

#include <glm/glm.hpp>

namespace Prefabs
{
entity_id createGround(World &world, int mesh_id, float size)
{
    entity_id entity = world.newEntity();

    TransformComponent transform{};
    transform.scale = glm::vec3(size, 1.0f, size); // 평면을 원하는 크기로 확장
    world.addComponent<TransformComponent>(entity, std::move(transform));

    RenderableComponent renderable{};
    renderable.mesh_id = mesh_id;
    renderable.color = {0.22f, 0.22f, 0.24f};
    renderable.use_grid = true; // 그리드 패턴 표시
    world.addComponent<RenderableComponent>(entity, std::move(renderable));

    return entity;
}
} // namespace Prefabs
