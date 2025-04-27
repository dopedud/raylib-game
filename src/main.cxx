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
    camera.position = (Vector3){ .0f, .0f, -8.0f };
    camera.target = (Vector3){ .0f, .0f, .0f };
    camera.up = (Vector3){ .0f, 1.0f, .0f };
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model model = LoadModel("../resources/monke.glb");

    Vector3 model_position { .0f, .0f, .0f };

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);
                DrawModelEx(model, model_position, { .0f, .0f, .0f }, 90.0f, { 1.0f, 1.0f, 1.0f }, WHITE);
                // DrawModel(model, model_position, 1.0f, WHITE);
                DrawGrid(25, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(model);

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

    SetExitKey(KEY_NULL);
}