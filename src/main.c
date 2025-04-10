#include <stdio.h>
#include <string.h>
#include <time.h>

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "logger.h"

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

void TimestampLogCallback(int logType, const char *text, va_list args);

int main()
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogCallback(TimestampLogCallback);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    int previous_screen_width = GetScreenWidth();
    int previous_screen_height = GetScreenHeight();

    Shader shader = LoadShader(0, "shaders/julia_set.fs");

    RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    
    float c[2] = { -0.348827f, 0.607167f };

    // Offset and zoom to draw the julia set at. (centered on screen and default size)
    float offset[2] = { 0.0f, 0.0f };
    float zoom = STARTING_ZOOM;

    // Get variable (uniform) locations on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    int c_loc = GetShaderLocation(shader, "c");
    int zoom_loc = GetShaderLocation(shader, "zoom");
    int offset_loc = GetShaderLocation(shader, "offset");

    // Upload the shader uniform values!
    SetShaderValue(shader, c_loc, c, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, zoom_loc, &zoom, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, offset_loc, offset, SHADER_UNIFORM_VEC2);

    while (!WindowShouldClose())
    {
        if (IsWindowResized)
        {
            int screen_width = GetScreenWidth();
            int screen_height = GetScreenHeight();

            if (previous_screen_width != screen_width || previous_screen_height != screen_height)
            {
                UnloadRenderTexture(target);

                target = LoadRenderTexture(screen_width, screen_height);

                previous_screen_width = screen_width;
                previous_screen_height = screen_height;
            }
        }

        // If "R" is pressed, reset zoom and offset.
        if (IsKeyPressed(KEY_R))
        {
            zoom = STARTING_ZOOM;
            offset[0] = 0.0f;
            offset[1] = 0.0f;
            SetShaderValue(shader, zoom_loc, &zoom, SHADER_UNIFORM_FLOAT);
            SetShaderValue(shader, offset_loc, offset, SHADER_UNIFORM_VEC2);
        }

        // If either left or right button is pressed, zoom in/out.
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            // Change zoom. If Mouse left -> zoom in. Mouse right -> zoom out.
            zoom *= IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? ZOOM_SPEED : 1.0f/ZOOM_SPEED;

            const Vector2 mousePos = GetMousePosition();
            Vector2 offsetVelocity;
            // Find the velocity at which to change the camera. Take the distance of the mouse
            // from the center of the screen as the direction, and adjust magnitude based on
            // the current zoom.
            offsetVelocity.x = (mousePos.x/(float)GetScreenWidth() - 0.5f)*OFFSET_SPEED_MULTIPLIER/zoom;
            offsetVelocity.y = (mousePos.y/(float)GetScreenHeight() - 0.5f)*OFFSET_SPEED_MULTIPLIER/zoom;

            // Apply move velocity to camera
            offset[0] += GetFrameTime()*offsetVelocity.x;
            offset[1] += GetFrameTime()*offsetVelocity.y;

            // Update the shader uniform values!
            SetShaderValue(shader, zoom_loc, &zoom, SHADER_UNIFORM_FLOAT);
            SetShaderValue(shader, offset_loc, offset, SHADER_UNIFORM_VEC2);
        }

        // Using a render texture to draw Julia set
        BeginTextureMode(target);       // Enable drawing to texture
            ClearBackground(BLACK);     // Clear the render texture

            // Draw a rectangle in shader mode to be used as shader canvas
            // NOTE: Rectangle uses font white character texture coordinates,
            // so shader can not be applied here directly because input vertexTexCoord
            // do not represent full screen coordinates (space where want to apply shader)
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);     // Clear screen background

            // Draw the saved texture and rendered julia set with shader
            // NOTE: We do not invert texture on Y, already considered inside shader
            BeginShaderMode(shader);
                // WARNING: If FLAG_WINDOW_HIGHDPI is enabled, HighDPI monitor scaling should be considered
                // when rendering the RenderTexture2D to fit in the HighDPI scaled Window
                DrawTextureEx(target.texture, (Vector2){ 0.0f, 0.0f }, 0.0f, 1.0f, WHITE);
            EndShaderMode();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadShader(shader);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
