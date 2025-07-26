# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Configuration
- **Debug build**: `cmake --preset debug src -B build`
- **Release build**: `cmake --preset release src -B build`
- **Fresh configuration** (when CMakeLists.txt changes): `cmake --fresh --preset debug src -B build`

### Building
- **Build project**: `cmake --build build`
- **Full clean** (removes build directory): `cmake --build build --target full_clean`
- **Full clean with dependencies**: `cmake --build build --target full_clean_dependency`

### Installation
- **Install to installs/ directory**: `cmake --build build --target install`
- **Clean install**: `cmake --build build --target clean_install`

### Running
- **Run game**: `run` (uses run.bat which executes `build\bin\game.exe`)

### Documentation
- **Generate docs**: `doxygen Doxyfile` (creates `docs/html/index.html`)

## Build Configuration

The project uses CMake with FetchContent to manage dependencies:
- **Build type**: Shared libraries (BUILD_SHARED_LIBS=ON) to keep executable size small
- **Dependencies location**: `_deps/` directory via FETCHCONTENT_BASE_DIR
- **Output directories**: 
  - Executables: `build/bin/`
  - Libraries: `build/lib/`
  - Install prefix: `installs/`

## Architecture Overview

### Core Systems
- **ResourceManager**: Singleton pattern managing game resources (textures, shaders, models, Box2D physics world)
- **StateManager**: Generic template class for state management using enums (with static_assert for type safety)
- **AnimatedModel**: Custom 2D sprite animation system applied to 3D models with sophisticated timing controls
- **Player**: Main game entity with physics body, animation, and input handling
- **PlayerCameraController**: Camera management for following player
- **Settings**: Namespace containing game configuration constants separated into GENERAL and PHYSICS

### Game Loop Structure
The main game loop in `main.cxx` follows this pattern:
1. **Cursor toggle handling**: ESC key to enable/disable cursor
2. **Physics simulation** (fixed timestep): Box2D world step with timestep accumulation
3. **Animation update**: Player animation state updates  
4. **Input handling**: Player input processing
5. **Rendering**: 3D scene with dual grids (XZ and Y planes), models, and FPS display

### Resource Management
- **Texture resources**: Organized by animation type (idle, run, slide) with frame sequences loaded via resourcevars namespace
- **Shader resources**: Custom GLSL shaders for rendering (vertex and fragment shaders)
- **Model resources**: 3D models with 1:1 relationship to texture resources (generated from cube meshes)
- **Physics bodies**: Box2D bodies managed through ResourceManager singleton
- **Resource paths**: Centralized in resourcevars namespace with FRAMES_AMOUNT, TEXTUREPATH, and SHADERPATH structs

### Key Design Patterns
- **Singleton**: ResourceManager for global resource access
- **Component composition**: Player combines physics body, animated model, and state management
- **Template-based state management**: StateManager works with any enum type
- **Resource enumeration**: Type-safe resource access through enums

### Directory Structure
- `src/`: All source code (.cxx, .h files)
- `src/resources/`: Game assets (textures, shaders, models)
- `build/`: CMake build output
  - `build/bin/`: Built executables
  - `build/lib/`: Static libraries
- `installs/`: Final installation directory
- `docs/`: Generated Doxygen documentation
- `_deps/`: External dependencies (raylib, box2d, imgui, rlimgui)
- `test/`: Test files

### Dependencies
- **Raylib 5.5**: Graphics and windowing
- **Box2D 3.1.0**: Physics simulation
- **Dear ImGui 1.92.1**: UI framework (migrated from RayGUI)
- **rlImGui**: Raylib-ImGui integration layer
- **C++17**: Language standard

### Physics Integration
- World managed by ResourceManager singleton
- Fixed timestep simulation (60 FPS physics, configurable display FPS)
- Box2D 3.1.0 bodies tied to game entities
- Ground plane for collision testing with proper collision shapes

### Animation System
- Custom AnimatedModel class for 2D sprite sequences on 3D models
- Timing-based frame progression
- Support for looping and non-looping animations
- Texture dimensions must be consistent within animation sets