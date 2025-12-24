2025-12-24 일단 성능은 포기한다. 하나씩 고치려다 보니까 너무 많다

---

2025-12-23 registry 구조 수정 필요
prefabs 생성 -> registry에서 addComponents로 하나씩 붙여주도록 되어있음
-> component 업데이트 시 캐시미스 발생 확률이 너무 높고, 하나씩 조립하기도 귀찮음

일반적인 ECS 프레임워크는 Archetype 기반의 레지스트리를 사용한다고 함
OpenGL VAO, VBO 처럼 chunk로 관리하는 형식

registry 이름도 모호한거 같아서 world로 바꿈
world의 책임은 다음과 같이 정함
- 모든 Entity와 Component 관리
- 이 데이터들을 효율적으로 저장하고, system들이 안전하게 접근할 수 있도록 함

---

2025-12-21 구조 점검
```
1. Compile-time dependency
┌──────────────────────────────┐
│        Application (APP)     │
│------------------------------│
│ Engine: frame orchestration  │
│ Prefabs: entity assembly     │
│ Input: event→command mapping │
│ (Game/Camera/Sim policies)   │
└──────────────┬───────────────┘
               │ depends on
               ▼
┌──────────────────────────────┐
│          ECS / World          │
│------------------------------│
│ Entity lifecycle             │
│  - Entity{index,gen}         │
│  - create/destroy/alive      │
│ Components storage           │
│  - Transform/Renderable/...  │
│ Systems (Update side)        │
│  - Simulation/Camera/...     │
│ RenderExtractSystem          │
│  - reads ECS → emits DrawCmd │
└──────────────┬───────────────┘
               │ uses ONLY neutral types
               ▼
┌──────────────────────────────┐
│     Render API (Neutral)      │
│------------------------------│
│ DrawCommand                   │
│  - mesh_h, material_h         │
│  - transform, sort_key        │
│ RenderQueue (frame-local)     │
│ Typed Handles (idx,gen)       │
└──────────────┬───────────────┘
               │ depends on
               ▼
┌──────────────────────────────┐
│        Graphics (GFX)         │
│------------------------------│
│ Renderer: consumes queue      │
│ ResourceDB: resolves handles  │
│  - Mesh/Material/Shader       │
│ Backend: GLFW+OpenGL          │
└──────────────────────────────┘

[Dependency Rule]
- ECS/World never includes OpenGL/GLFW/Mesh/Shader headers
- Graphics never includes Registry/Components headers
- The only bridge is Render API (DrawCommand/Handles/RenderQueue)


2. Runtime FRAME LOOP
(0) OS/Window System
    │
    ▼
(1) Backend (GLFW)
    - PollEvents()
    - raw input events 발생
    │
    ▼
(2) Input Queue (APP)
    - InputEvent들을 저장 (Key, Mouse, Time, Mods...)
    - "직접 컨트롤러 호출" 금지 (필수 분리 지점)
    │
    ▼
(3) Command Mapping (APP)
    - InputEvent → GameCommand 변환
      ex) W down → MoveForward(start)
          mouse dx/dy → CameraLook(delta)
          space → ToggleMode
    │
    ▼
(4) Update / Simulation (ECS Systems)
    - World/Components 갱신
      Transform, Velocity, Camera, AI state...
    - Entity alive 체크로 stale 방지
    │
    ▼
(5) Render Extract (ECS side, CPU)
    - Renderable Entity List(연속 벡터) 순회
    - 필요한 컴포넌트 읽기(Transform/Renderable)
    - DrawCommand 생성 + RenderQueue push
    │
    ▼
(6) Render Submit (GFX side, GPU)
    - Renderer consumes RenderQueue
    - ResourceDB resolves handles(idx,gen)
    - Bind material/shader, bind mesh, set uniforms
    - glDraw* 호출
    │
    ▼
(7) SwapBuffers / Present
    - 화면 출력
    │
    └─────────────── 다음 프레임으로 반복 ────────────────►
```
---

2025-12-21 TODO(jyan): 추후 보완해야할 부분 정리
- Entity 생명주기: Entity{index, gen} + create/destroy/alive + removeComponent. ID 재사용 안전 필수
- 연속 데이터 순회: ComponentArray에 entity_ids 병렬 배열 추가 → forEachRenderable 등은 map 대신 이 배열을 순회
- Render API 확장: DrawCommand = {mesh_h, material_h, transform, sort_key, flags} + typed handle(idx, gen)
- ResourceDB 분리: Mesh/Material/Shader를 핸들로 등록/조회하는 DB를 Renderer 외부에 두고, Renderer는 RenderQueue만 소비
- Material 분리: 색상 등 머티리얼 속성을 Renderable에서 분리 → Material 테이블(최소 baseColor)로 이동. 이후 텍스처/PBR 확장
- 입력 파이프: 이벤트 큐 → 커맨드 매핑 → 시스템 업데이트. GLFW 콜백이 컨트롤러를 직접 호출하지 않도록 경계 설정
- 디버그/릴리스: per-draw glGetError 제거, KHR_debug/디버그 플래그로 한정. 기본 상태(cull face 등) 초기 설정

---

2025-12-11
프로젝트 구조 설계중..
기존 설계 구조는 다음과 같이 헥사고날 아키텍처를 이용하려고 했음(테스트와 고도의 추상화를 통해 아름다운 설계를 하고싶었음. 세미나를 인상깊게 듣기도 해서.. 최대한 참고하여 설계함)
  ```
  jyan@jyan:$ tree
  .
  ├── adapter
  │   ├── CMakeLists.txt
  │   ├── in
  │   │   ├── CMakeLists.txt
  │   │   ├── include
  │   │   │   └── controller.hpp
  │   │   └── src
  │   │       └── controller.cpp
  │   └── out
  ├── application
  │   ├── CMakeLists.txt
  │   ├── domain
  │   │   ├── CMakeLists.txt
  │   │   ├── event
  │   │   ├── model
  │   │   └── service
  │   │       ├── CMakeLists.txt
  │   │       ├── include
  │   │       │   └── engine.hpp
  │   │       └── src
  │   │           └── engine.cpp
  │   └── port
  │       ├── CMakeLists.txt
  │       ├── in
  │       └── out
  │           ├── CMakeLists.txt
  │           └── interface
  │               └── render_port.hpp
  ├── assembly
  │   ├── CMakeLists.txt
  │   ├── include
  │   │   └── application_builder.hpp
  │   └── src
  │       └── application_builder.cpp
  └── main.cpp
```

실제 코드 작성하다보니 domain -> adapter로 데이터를 계속해서 복사하거나 참조해야함 + 가상 함수를 계속해서 호출해야함
=> 가상함수 호출 오버헤드 + 캐시 미스 발생할 확률이 높다고 판단(렌더링에 필요한 데이터의 위치: application/domain/model과 실제 렌더링을 수행하는 곳: adapter)
=> 구조를 아래와 같이 수정: 헥사고날 아키텍처의 장점인 로직 분리를 일부 유지하면서 렌더링 파이프라인의 성능을 고려해봄. 이전에 게임 만들때 rust, bevy에서 사용하던 구조 참고함
```
jyan@jyan:$ tree
.
├── application
│   ├── engine.cpp
│   └── engine.hpp
├── ecs
│   ├── component.hpp
│   ├── registry.hpp
│   └── render_system.hpp
├── graphics
│   ├── render_data.hpp
│   ├── renderer.cpp
│   ├── renderer.hpp
│   └── shaders
│       ├── shader.hpp
│       ├── shader_fragment
│       └── shader_vertex
└── main.cpp
```