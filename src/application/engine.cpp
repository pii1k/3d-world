#include "application/engine.hpp"

#include <iostream>

#include "../ecs/component.hpp"
#include "../ecs/registry.hpp"
#include "../ecs/render_system.hpp"
#include "../graphics/renderer.hpp"

void Engine::run()
{
    Registry registry;          // ECS Registry 생성
    RenderSystem render_system; // 렌더 시스템 준비
    Renderer renderer;          // 실제 렌더러 객체

    if (!renderer.init(1280, 720, "Automotive Simulator")) // GLX 창 및 OpenGL 초기화
    {
        std::cerr << "Renderer initialization failed\n"; // 실패 로그
        return;                                          // 더 이상 진행하지 않음
    }

    Entity cube = registry.createEntity();                                 // 첫 번째 큐브 엔티티 생성
    registry.addTransform(cube, TransformComponent{{-1.5F, 0.0F, -5.0F}}); // 위치 설정
    registry.addRenderable(cube, RenderableComponent{0});                  // 큐브 모델 ID 부여

    Entity cube2 = registry.createEntity();                                // 두 번째 큐브 생성
    registry.addTransform(cube2, TransformComponent{{1.5F, 0.5F, -6.0F}}); // 위치 설정
    registry.addRenderable(cube2, RenderableComponent{0});                 // 동일한 큐브 메시 사용

    while (!renderer.windowShouldClose()) // 창이 열려있는 동안 루프
    {
        RenderQueue render_queue;                     // 프레임별 렌더 큐
        render_system.update(registry, render_queue); // ECS 데이터를 draw command로 변환
        renderer.draw(render_queue);                  // 큐를 실제로 렌더링
        renderer.swapBuffers();                       // 새 프레임을 화면에 표시
        renderer.pollEvents();                        // X 이벤트 처리 및 종료 여부 업데이트
    }
}