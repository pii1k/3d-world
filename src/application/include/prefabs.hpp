// 프리팹: 컴포넌트 조합을 한 번에 세팅하는 헬퍼
#pragma once

#include "registry.hpp"

namespace Prefabs
{
Entity createPlayer(Registry &registry, int mesh_id);
Entity createGround(Registry &registry, int mesh_id, float size);
} // namespace Prefabs