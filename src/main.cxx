__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

#ifdef PLATFORM_DESKTOP
    #define GLSL_VERSION    330
#else // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION    100
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main()
{
    ChangeDirectory(GetApplicationDirectory());

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    while (!WindowShouldClose())
    {
        
    }

    CloseWindow();

    return 0;
}
