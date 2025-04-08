#include <string.h>

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void UpdateDrawFrame();

int main()
{
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

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}
