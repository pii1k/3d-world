# 3d-world

A small C++/OpenGL playground for experimenting with simple 3D games and systems.
The project is organized so each game lives under `src/domain/<game_name>` and is built via a shared engine/renderer.

## Build

This project uses CMake presets. Both `build.sh` and VS Code's CMake Tools build against the same preset definitions in [CMakePresets.json](/home/jyan/cpp/3d-world/CMakePresets.json).

Build for the current host OS with the default game (`bat_game`):

```bash
./build.sh
```

Build a specific game:

```bash
./build.sh --game=bat_game, snake_game
```

Build for a specific target OS:

```bash
./build.sh --os=linux
./build.sh --os=mac
./build.sh --os=windows, window
```

Clean and rebuild:

```bash
./build.sh --clean
```

You can also use CMake directly with presets:

```bash
cmake --preset host-debug
cmake --build --preset host-debug
```

Available configure/build presets:

- `host-debug` - debug build for the current machine's OS
- `linux-debug` - debug build with `TARGET_OS=linux`
- `mac-debug` - debug build with `TARGET_OS=mac`
- `windows-debug` - debug build with `TARGET_OS=windows`

To override the game when using CMake directly:

```bash
cmake --preset host-debug -DGAME_NAME=snake_game
cmake --build --preset host-debug
```

In VS Code, use the CMake Tools extension and select one of the same presets. The workspace default is `host-debug`.

## Run

When building through presets, the executable is placed under the preset-specific build directory:

```bash
./build/host-debug/src/main_app
```

Examples:

```bash
./build/linux-debug/src/main_app
./build/windows-debug/src/main_app
```

## Project Layout

- `src/app` — application entry point
- `src/engine` — windowing, loop, and core engine glue
- `src/graphics` — basic renderer/shader setup
- `src/domain` — game implementations (e.g., `snake_game`, `bat_game`)
~~- `src/input` — input helpers/controllers~~
- `src/util` — utilities

---

## [2026-03-22]Notes
- Use the `GAME_NAME` CMake option to switch which game is built.
- `build.sh` maps `window` and `windows` to the same target OS.
- The default shader is a simple color shader used for prototyping.

## TODO / Improvements

- Add a proper asset pipeline (models, textures, audio).
- Introduce per-game shader sets and materials.
- Add a debug UI/overlay (FPS, camera info, input state).
- Improve input handling (mouse look, remapping, sensitivity).
- Add basic physics helpers (gravity, collisions).
- Standardize game module interfaces and data flow.
- Add tests or lightweight validation tools where feasible.
