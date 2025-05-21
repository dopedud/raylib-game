__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include <iostream>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "box2d/box2d.h"

#include "settings.h"
#include "player.h"

int main(int argc, char* argv[])
{
    initialise();

    b2WorldDef physical_world = b2DefaultWorldDef();

    b2SetLengthUnitsPerMeter(TEXELS_PER_UNIT);
	physical_world.gravity.y = GRAVITY_ACCELERATION * TEXELS_PER_UNIT;

	b2WorldId worldId = b2CreateWorld(&physical_world);

    Camera3D camera;
    camera.position = { .0f, .1f, -5.0f };
    camera.target = { .0f, .0f, .0f };
    camera.up = { .0f, 1.0f, .0f };
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model dummy { LoadModel("../resources/monke.glb") };

    Player player {};

    camera.target = Vector3Zeros;

    SetTargetFPS(512);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        player.update();

        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                player.draw();
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(dummy);

    CloseWindow();

    return 0;
}