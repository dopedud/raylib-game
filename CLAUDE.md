# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

**Quick build and run:**
```bash
run
```
This runs the batch file that executes: `cmake --build build && cmake --install build && installs\bin\game.exe`

**Development workflow:**
```bash
# Configure with user preset (required first time or after CMakeLists.txt changes)
cmake --fresh --preset user-debug . -B build

# Build the project
cmake --build build

# Install and run
cmake --install build
installs\bin\game.exe
```

**Cleanup commands:**
```bash
# Clean install directory
cmake --build build --target clean_install

# Full clean (removes build directory)
cmake --build build --target full_clean

# Full clean including dependencies
cmake --build build --target full_clean_dependency
```

**Documentation generation:**
```bash
# Generate architecture diagram (requires PlantUML)
java -jar plantuml-1.2025.4.jar architecture.puml -o docs

# Generate code documentation (requires Doxygen)
doxygen Doxyfile
```

## Architecture Overview

This is a 3D game built with Raylib (graphics) and Box2D (physics), written in C++17. The project follows a component-based architecture with singleton resource management.

### Core Systems

**Resource Management (Singleton)**
- `ResourceManager` - Manages textures, shaders, models, and Box2D world
- `constants.h` - Defines resource enumerations and paths
- Resources organized by type with enum-based indexing

**Game State System**
- `StateManager<E>` - Generic template-based state management
- `Player` uses `StateManager<PlayerState>` for IDLE/MOVING/JUMPING states
- State transitions are planned but not fully implemented

**Animation System**
- `AnimatedModel` - 2D texture animation on 3D models
- Keyframe-based timing with configurable frame durations
- Supports looping and non-looping animations

**Physics Integration**
- Box2D world managed by ResourceManager
- Fixed timestep physics simulation (60Hz with 4 substeps)
- Player has multiple physics bodies for different states (partially implemented)

**Camera System**
- `CameraController` - Basic camera handling
- `PlayerCameraController` - Intended for player following (incomplete)

### Key Files Structure

```
src/
├── main.cxx              # Main game loop, physics, and rendering
├── resource_manager.*    # Singleton resource management
├── state_manager.*       # Generic state management template
├── player.*              # Player entity with physics and animation
├── animated_model.*      # 2D animation on 3D models
├── camera_controller.*   # Camera handling
├── settings.*            # Game configuration constants
└── editor/               # Debug editor (ImGui-based)
```

### Known Issues & Incomplete Features

**Critical Issues:**
- Player state transitions never occur (always stays IDLE)
- Camera following is not implemented (`PlayerCameraController::follow()` is empty)
- Multiple physics bodies per player created but never switched
- MOVING and JUMPING animation states are empty

**Architecture Notes:**
- DEBUG mode enables ImGui editor and different render path
- Physics simulation runs at fixed 60Hz timestep
- Input polling at 512Hz for responsive controls
- Game runs at 512 FPS target with physics decoupled

### Development Patterns

**Resource Loading:**
- Enums define resource types (TextureResource, ShaderResource, ModelResource)
- Paths in `resourcevars` namespace with structured naming
- Animation frames loaded from numbered file sequences

**Memory Management:**
- Singleton pattern for ResourceManager with thread-safe initialization
- RAII patterns throughout codebase
- Proper cleanup in destructors

**Build System:**
- CMake with FetchContent for dependencies (Raylib, ImGui, Box2D)
- Separate debug/release presets with different compiler flags
- User-specific presets in `CMakeUserPresets.json`