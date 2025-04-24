# Game from Scratch in C/C++ via Raylib and Bullet

## About

This is the git repository for a project about making a game from scratch, written in C and C++ powered by
[Raylib](https://www.raylib.com/index.html) ([Github link](https://github.com/raysan5/raylib?tab=readme-ov-file)) and
physics engine powered by [Bullet](https://pybullet.org/wordpress/) ([Github link](https://github.com/bulletphysics/bullet3)).

## Running the Game

Use the `run.bat` file via typing in console `run`. There's nothing special about `run.bat`; it only acts as an alias
to the full path to the executable `build\lib\main.exe`.

## Development Notes

## Compilation Notes

This project uses CMake to build its files. Therefore, CMake is required to build this project. All the build
parameters and overall settings are written in `CMakeLists.txt`, and build presets are written in `CmakePresets.json`.
All CMake operations are done with a console, with this project's root folder as its working directory.

Build files are required to build this project, and CMake will first need to configure those build files. To configure,
type in `cmake --preset debug src -B build` in console.

The `--preset` command allows the user to choose which configuration or build preset should CMake use for this project.
This can range from determining which build generators to use (for this project it's `MinGW Makefiles`), to setting the
cache variables before reading `CMakeLists.txt`. In the case of this project, it's only used to specify which build
generator should CMake use, and the compiler flags for both debug and release build respectively.

Optionally, the `--fresh` argument should be used if `CMakeLists.txt` file was changed. The `--fresh` argument rewrites
the `CMakeCache.txt` file which stores various variables and their values between CMake runs.

After configuration, CMake can then build the project with the generated build files. To build, type in `cmake --build
build` in console.

In addition, if it is required to do a full clean of the build (basically deleting the whole `build` directory), the
command `cmake --build build --target full_clean` should be used. It uses the custom target made in `CMakeLists.txt`
file to delete the entire `build` directory to build this project from scratch. It may throw errors and the end of
command execution, which is an expected behaviour. One can then enter the configuration stage, and proceed from there.
One can also delete the `_deps` directory (the directory where this project's dependencies reside) via the target `full_clean_dependency`.

common commands (for easy copy):
- `cmake --build build --target clean_install`
- `cmake --build build --target full_clean`
- `cmake --build build --target full_clean_dependency`
- `cmake --fresh --preset debug src -B build`
- `cmake --build build`
