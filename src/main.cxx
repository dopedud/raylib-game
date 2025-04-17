__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include <vector>
#include <memory>

extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

#include "btBulletDynamicsCommon.h"

#include "logger.h"



int main(int argc, char* argv[])
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogCallback(TimestampLogCallback);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    while (!WindowShouldClose())
    {
        
    }

    CloseWindow();

    return 0;
}