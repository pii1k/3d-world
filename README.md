# 3d-world

A small C++/OpenGL playground for experimenting with simple 3D games and systems.
The project is organized so each game lives under `src/domain/<game_name>` and is built via a shared engine/renderer.

## Build

```bash
./build.sh
```

To build a specific game:

```bash
./build.sh --game=bat_game
```

## Run

```bash
./build/src/main_app
```

## Project Layout

- `src/app` — application entry point
- `src/engine` — windowing, loop, and core engine glue
- `src/graphics` — basic renderer/shader setup
- `src/domain` — game implementations (e.g., `snake_game`, `bat_game`)
- `src/input` — input helpers/controllers
- `src/util` — utilities

## Notes

- Use the `GAME_NAME` CMake option to switch which game is built.
- The default shader is a simple color shader used for prototyping.

## TODO / Improvements

- Add a proper asset pipeline (models, textures, audio).
- Introduce per-game shader sets and materials.
- Add a debug UI/overlay (FPS, camera info, input state).
- Improve input handling (mouse look, remapping, sensitivity).
- Add basic physics helpers (gravity, collisions).
- Standardize game module interfaces and data flow.
- Add tests or lightweight validation tools where feasible.