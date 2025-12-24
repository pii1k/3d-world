#include "prefabs.hpp"
#include "component.hpp"

#include <glm/glm.hpp>

namespace Prefabs
{
entity_id createPlayer(World &world, int mesh_id)
{
    entity_id entity = world.newEntity();
    TransformComponent transform{};
    transform.position = {0.0f, 0.5f, 0.0f};
    world.addTransform(entity, transform);

    RenderableComponent renderable{};
    renderable.mesh_id = mesh_id;
    renderable.color = {1.0f, 0.5f, 0.2f}; // 주황 큐브
    world.addRenderable(entity, renderable);

    PhysicsComponent physics{};
    world.addComponent(entity, physics);

    PlayerControllerComponent player_contoller{};
    world.addComponent(entity, player_contoller);

    return entity;
}

entity_id createGround(World &world, int mesh_id, float size)
{
    entity_id entity = world.newEntity();

    TransformComponent transform{};
    transform.scale = glm::vec3(size, 1.0f, size); // 평면을 원하는 크기로 확장
    world.addTransform(entity, transform);

    RenderableComponent renderable{};
    renderable.mesh_id = mesh_id;
    renderable.color = {0.22f, 0.22f, 0.24f};
    renderable.use_grid = true; // 그리드 패턴 표시
    world.addRenderable(entity, renderable);

    return entity;
}
} // namespace Prefabs