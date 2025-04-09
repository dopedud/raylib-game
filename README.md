# Game Engine from Scratch in C via Raylib

## About

This is the git repository for a C project about making a game from scratch, written in C powered by
[Raylib](https://www.raylib.com/index.html) ([Github link](https://github.com/raysan5/raylib?tab=readme-ov-file)).

## Features

- TBA

## Compilation Notes

This project uses CMake to build its files. Therefore, CMake is required to build this project. All the build
parameters and overall settings are written in `CMakeLists.txt`, and build presets are written in `CmakePresets.json`.
All CMake operations are done with a console, with this project's root folder as its working directory.

Build files are required to build this project, and CMake will first need to configure those build files. To configure,
type in `cmake --fresh --preset=default src -B build` in console.

`--fresh` argument refreshes the build files so that new build files can be written (this argument can be optional,
thus speeds up build configuration times, but any changes made in `CMakeLists.txt` will require the argument to be included).

`--preset` argument indicates which build preset should be used in `CMakePresets.json` (if there's any). `default` is
set as the build preset (this argument is used to determine what build generator should be used, which depends on what
compiler is installed on the current computer).

After configuration, CMake can then build the project with the generated build files. To build, type in `cmake --build
build` in console.

common commands (for easy copy):
- `cmake --preset=default src -B build`
- `cmake --build build`