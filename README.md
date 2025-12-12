2025-12-11
프로젝트 구조 설계중..
기존 설계 구조는 다음과 같이 헥사고날 아키텍처를 이용하려고 했음(테스트와 고도의 추상화를 통해 아름다운 설계를 하고싶었음. 세미나를 인상깊게 듣기도 해서.. 최대한 참고하여 설계함)
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

실제 코드 작성하다보니 domain -> adapter로 데이터를 계속해서 복사하거나 참조해야함 + 가상 함수를 계속해서 호출해야함
가상함수 호출 오버헤드 + 캐시 미스 발생할 확률이 높다고 판단(렌더링에 필요한 데이터의 위치: application/domain/model과 실제 렌더링을 수행하는 곳: adapter)
=> 구조를 아래와 같이 수정: 헥사고날 아키텍처의 장점을인 로직 분리를 일부 유지하면서 렌더링 파이프라인의 성능을 고려해봄. 이전에 게임 만들때 rust, bevy에서 사용하던 구조 참고함
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