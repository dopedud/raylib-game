#include <string.h>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/raygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void UpdateDrawFrame();

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    while (!WindowShouldClose())
    {
        DrawFPS(10, 10);
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

void UpdateDrawFrame()
{
    BeginDrawing();

        Texture2D tex2d = LoadTextureFromImage(LoadImage("image.jpg"));
        DrawTexture(tex2d, 10, 10, { Color });

        ClearBackground(RAYWHITE);

    EndDrawing();
}
