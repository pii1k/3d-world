// 프리팹: 컴포넌트 조합을 한 번에 세팅하는 헬퍼
#pragma once

#include "world.hpp"

namespace Prefabs
{
entity_id createPlayer(World &world, int mesh_id);
entity_id createGround(World &world, int mesh_id, float size);
} // namespace Prefabs