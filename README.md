# Game from Scratch in C/C++ via Raylib and Box2D

## About

This is the git repository for a project about making a game from scratch, written in C and C++ powered by
[Raylib](https://www.raylib.com/index.html) ([Github link](https://github.com/raysan5/raylib?tab=readme-ov-file)) and
physics engine powered by [Box2D](https://box2d.org/) ([Github link](https://github.com/erincatto/box2d)).

## Compiling and Running the Game

Use the `run.bat` file via typing `run` in terminal. It will run `cmake --build build`, `cmake --install build`, and
`installs\bin\game` on the console to build and run the game accordingly.

## Compilation Notes

This project uses [CMake](https://cmake.org/) to build its files. Therefore, CMake is required to build this project.
All the build parameters and overall settings are written in `CMakeLists.txt`, project-wide build presets are written in
`CmakePresets.json`, and user-specific build presets are written in `CMakeUserPresets.json`. All CMake operations are
done within the console, with this project's root directory as its
working directory.

Build files are required to build this project, and CMake will first need to configure those build files. To configure,
you first need to have `CMakeUserPresets.json` that defines what Make generators are available in your system for
CMake to use. One such `CMakeUserPresets.json` is as shown below:

```json
{
    "name": "user-debug",
    "inherits": "debug",
    "displayName": "Debug Config (User)",
    "generator": "MinGW Makefiles",
    "cacheVariables":
    {
        "CMAKE_C_COMPILER": "C:/msys64/ucrt64/bin/gcc.exe",
        "CMAKE_CXX_COMPILER": "C:/msys64/ucrt64/bin/g++.exe",
        "CMAKE_MAKE_PROGRAM": "C:/msys64/mingw64/bin/mingw32-make.exe"
    }
},

{
    "name": "user-release",
    "inherits": "release",
    "displayName": "Release Config (User)",
    "generator": "MinGW Makefiles",
    "cacheVariables":
    {
        "CMAKE_C_COMPILER": "C:/msys64/ucrt64/bin/gcc.exe",
        "CMAKE_CXX_COMPILER": "C:/msys64/ucrt64/bin/g++.exe",
        "CMAKE_MAKE_PROGRAM": "C:/msys64/mingw64/bin/mingw32-make.exe"
    }
}
```

Both of these JSON objects are put inside `configurePresets` JSON array. Otherwise, `CMakeUserPresets.json` follows the
same JSON structure as `CMakePresets.json`. Note that you can define your own `CMakeUserPresets.json`, the ones shown
above are just an example.

The `--preset` command allows you to choose which configuration or build preset should CMake use for this project. This
can range from determining which build generators to use (for this project it's `MinGW Makefiles`), to setting the
cache variables before reading `CMakeLists.txt`. In the case of this project, it's used to both specify which build
generator should CMake use, and the compiler flags for both debug and release build respectively.

Optionally, the `--fresh` argument should be used if `CMakeLists.txt` file was changed. The `--fresh` argument rewrites
the `CMakeCache.txt` file which stores various variables and their values between CMake runs.

After configuration, CMake can then build the project with the generated build files. To build, type in `cmake --build
build` in console.

In addition, if you require a full clean of the build (basically deleting the whole `build` directory), the command
`cmake --build build --target full_clean` should be used. It uses the custom target made in `CMakeLists.txt` file to
delete the entire `build` directory to build this project from scratch. It may throw errors and the end of command
execution, which is an expected behaviour. You can then enter the configuration stage, and proceed from there. You can
also delete the `_deps` directory (the directory where this project's dependencies reside) via the target
`full_clean_dependency`.

Common commands (for easy copy):

```bash
cmake --build build --target clean_install
cmake --build build --target full_clean
cmake --build build --target full_clean_dependency
cmake --fresh --preset user-debug . -B build
cmake --build build
```

## Architecture Overview

### Tech Stack

- **Programming Languages:** C and C++
- **Rendering, Audio, Input Handling, and Animations:** Raylib
- **Debug GUI:** Dear IMGUI
- **Game GUI:** TBA
- **Physics Simulation:** Box2D


### Project Structure

```bash
src/
├─ main.cxx                # main game loop, physics, and rendering
├─ resource_manager.*      # Singleton resource management
├─ state_manager.*         # generic state management template
├─ player.*                # player entity with physics and animation
├─ animated_model.*        # 2D animation on 3D models
├─ camera_controller.*     # camera handling
├─ settings.*              # game configuration constants
├─ editor/                 # debug editor (ImGui-based)
└─ resources/              # game resources (audio, images, fonts, shader files,etc.)
```

### Architecture Diagram

Overview of the architecture of this project can be illustrated by the diagram below:

![](docs/architecture.png "architecture")

More information can be found by opening the documentation under the `docs` directory. You must first generate the
`docs` directory with Doxygen by following the steps described [here](#documentation-generation).

### Important Notes

This game has a feature in debug builds where you can switch between the normal view and the debug view, where Dear
IMGUI comes into play and provide an editor-like experience to debug the game, without any code recompilation going on
between mode transitions. This does mean that essentially you're not building the game in a game engine. You're just
tweaking variables that you've already compiled in the code to be edited in the game in debug builds. This also means
that the main development environment would still largely be building via code.

## Documentation Generation

To generate this project's architecture diagram, [PlantUML](https://plantuml.com/) must be installed. Version
`1.2025.4` is used to generate the diagram. It is packaged as a `.jar` file, and you can place it in this project's
root directory. To generate the diagram, type in the command below in command prompt:

```bash
java -jar plantuml-1.2025.4.jar architecture.puml -o docs
```

To generate this project's documentation, [Doxygen](https://www.doxygen.nl/index.html) must be installed. You can then
build the documentation files by entering the command below in command prompt with this project's root directory as the
working directory:

```bash
doxygen Doxyfile
```

The `docs` directory will then be modified, and Doxygen provides different forms of output to view the documentation.
For this project, a static HTML page will suffice, and can be accessed under:

```bash
docs\html\index.html
```

## Code Conventions

- ALL_CAPS for `const` or `constepxr` variables and enum values
- PascalCase for classes and enumerations
- snake_case for namespaces, functions, variables, and file names
- 4-space indentation
- Allman brace style (opening and closing brace in newline)

**Note:** Sometimes namespaces that should be snake_case are instead written in lowercase to avoid verbosity in code,
granted that it can still convey the same message. For example, in `src/constants.h`, the namespace `resourcevars` should be
`resource_variables` in snake_case, but in lowercase plus some character truncation it still contains the same meaning.

## Development Issues

There are issues to be addressed in this project, but for now I am too tired to list them out.
