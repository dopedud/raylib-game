# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

**Quick Build & Run:**
```bash
run.bat
```
This builds, installs, and runs the game in one command.

**Manual Build Process:**
```bash
# Configure (with existing CMakeUserPresets.json)
cmake --fresh --preset user-debug . -B build

# Build
cmake --build build

# Install dependencies and executable
cmake --install build

# Run
installs\bin\game.exe
```

**Clean Commands:**
```bash
# Clean install directory only
cmake --build build --target clean_install

# Full clean (build + install directories)
cmake --build build --target full_clean

# Full clean including dependencies
cmake --build build --target full_clean_dependency
```

## Architecture Overview

This is a 3D game built with Raylib (graphics), Box2D (physics), and ImGui (UI). The codebase follows a component-based architecture with clear separation of concerns:

**Core Systems:**
- `ResourceManager` - Singleton managing textures, shaders, models, and Box2D world
- `StateManager<E>` - Template class for state management with any enum type
- `CameraController` - 3D camera with smooth movement and mouse controls
- `Player` - Main character with physics bodies, animations, and state management
- `AnimatedModel` - 2D sprite animation system applied to 3D models

**Key Dependencies:**
- Raylib 5.5 (graphics/windowing)
- Box2D 3.1.0 (physics simulation)
- ImGui 1.92.1 (debug UI)
- rlImGui (Raylib-ImGui integration)

**Physics Integration:**
- Fixed timestep physics simulation (60Hz)
- High-frequency input polling (512Hz)
- Separate Box2D bodies for different player states

**Resource Loading:**
All resources use relative paths from `../resources/` directory. The ResourceManager loads:
- Animated textures (warrior sprites: idle, run, slide)
- Custom shaders (GLSL vertex/fragment)
- 3D models with texture mapping
- Fonts (Cascadia Code)

**Build System:**
- CMake with FetchContent for dependency management
- Shared libraries to keep executable size small
- Cross-platform support with Windows-specific optimizations
- Custom targets for cleaning build artifacts

## Known Issues

The README documents several incomplete systems:
- Player state transitions never occur (always IDLE)
- Camera follow() function is empty
- Multiple physics bodies created but only first one used
- MOVING/JUMPING animation states are incomplete
- Missing ground detection and collision feedback

## Development Notes

- All source files are in `src/` with corresponding headers
- Resources are duplicated in both `src/resources/` and `installs/resources/`
- Documentation generated with Doxygen (`doxygen Doxyfile`)
- Architecture diagram available via PlantUML (`architecture.puml`)
- Uses C++17 standard with GCC compiler