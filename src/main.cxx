__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include <iostream>

extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

#include "box2d/box2d.h"

#include "logger.h"

void initialise();

constexpr int SCREEN_WIDTH { 1280 };
constexpr int SCREEN_HEIGHT { 720 };

int main(int argc, char* argv[])
{
    initialise();

    // Define the camera to look into our 3d world
    Camera3D camera;
    camera.position = (Vector3){ 3.0f, 5.0f, -10.0f };
    camera.target = (Vector3){ .0f, .0f, .0f };
    camera.up = (Vector3){ .0f, 1.0f, .0f };
    camera.fovy = 5.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;

    Vector3 cube_position { 0.0f, 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cube_position, 2.0f, 2.0f, 2.0f, RED);

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initialise()
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogLevel(LOG_DEBUG);
    SetTraceLogCallback(TimestampLogCallback);

    SetRandomSeed(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}