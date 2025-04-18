__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include <cmath>
#include <ctime>

extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

#include "btBulletDynamicsCommon.h"

#include "logger.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;



int main(int argc, char* argv[])
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogCallback(TimestampLogCallback);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

                DrawCircle(540, 1080, 1080, BLUE);

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}