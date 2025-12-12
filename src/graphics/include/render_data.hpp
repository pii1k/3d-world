// RenderSystem이 만들어낼 결과물이자, OpenGLRenderer가 필요로 할 입력 데이터의 형식 정의. 이것이 바로 로직과 렌더링을 분리하는 다리 역할을 함
#pragma once

#include <glm/glm.hpp>
#include <vector>

using Matrix4x4 = glm::mat4;

// 렌더러에게 전달할 단일 그리기 명령
struct DrawCommand
{
    int model_id;
    Matrix4x4 transform;
};
// 프레임당 모든 그리기 명령의 목록
using RenderQueue = std::vector<DrawCommand>;