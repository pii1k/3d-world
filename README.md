# An experimental OpenGL/GLFW-based 3D rendering + ECS project

## Features

- ECS (World/Component/System) update loop
- Third-person camera (mouse look + follow)
- Basic primitive mesh rendering (Plane/Cube)
- GLSL shader loading (assets included)

## Requirements

- CMake 3.20+
- C++20 compiler
- OpenGL
- GLM (header-only library)
- Network: GLFW is downloaded via CMake FetchContent during the initial configure step.

## Getting Started (Ubuntu/WSL)

Install build tools and OpenGL/GLFW system dependencies:

```bash
sudo apt update && sudo apt install -y \
  build-essential cmake pkg-config \
  libgl1-mesa-dev libglu1-mesa-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

## Build & Run

```bash
cmake -S . -B build
cmake --build build -j
./build/3d-world
```

## Controls

- `Scroll`: Zoom (camera distance)
- `Left Click`: show entity information
- `Esc`: Quit

## Project Layout

- `src/application`: Engine loop / scene setup / controller / ui (Prefabs for MVP)
- `src/graphics`: Renderer / camera / mesh / shaders
- `src/ecs`: ECS interfaces (components / entity / systems)