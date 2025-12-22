#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// 3d 공간에서의 위치, 회전, 크기를 나타냄
struct TransformComponent
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // w, x, y, z
    glm::vec3 scale = glm::vec3(1.0f);

    // 이 컴포넌트의 최종 변환 행렬을 계산해서 반환하는 헬퍼 함수
    glm::mat4 getTransform() const
    {
        glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
        return glm::translate(glm::mat4(1.0f), position) * rotation_matrix * glm::scale(glm::mat4(1.0f), scale);
    }
};

// 렌더링이 가능한 객체임을 나타냄 지금은 어떤 모델을 그릴지만 간단히 정의
struct RenderableComponent
{
    int mesh_id = 0;                   // 렌더러가 이해할 수 있는 메시 ID
    glm::vec3 color{1.0f, 0.5f, 0.2f}; // 기본 색상 (주황)
    bool use_grid = false;             // 평면에 격자 패턴을 입힐지 여부
};

enum class LightType
{
    Directional,
    Point,
    Spot,
};

struct LightComponent
{
    LightType type{LightType::Directional};
    glm::vec3 color{1.0f};
    float intensity{1.0f};
    glm::vec3 position{0.0f};
    glm::vec3 direction{0, -1, 0};
    float range{15.0f};
    float inner_cone{0.85f};
    float outer_cone{0.9f};
    bool enabled{true};
};