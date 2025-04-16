__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

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
