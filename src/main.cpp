#include <stdio.h>
#include <string.h>
#include <time.h>

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
extern "C"
{
    #include "raygui.h"
}

#include "logger.hpp"

#ifdef PLATFORM_DESKTOP
    #define GLSL_VERSION    330
#else // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION    100
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define STARTING_ZOOM 1
#define ZOOM_SPEED 1.01f
#define OFFSET_SPEED_MULTIPLIER 2.0f

int main()
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogCallback(TimestampLogCallback);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    CloseWindow();

    return 0;
}
