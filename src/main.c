#include <stdio.h>
#include <string.h>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/raygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main()
{
    ChangeDirectory(GetApplicationDirectory());

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    Image image = LoadImage("../resources/image.jpg");
    Texture2D tex2d = LoadTextureFromImage(image);
    UnloadImage(image);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(
                tex2d, 
                SCREEN_WIDTH/2 - tex2d.width/2, 
                SCREEN_HEIGHT/2 - tex2d.height/2, 
                WHITE
            );

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(tex2d);
    CloseWindow();

    return 0;
}
