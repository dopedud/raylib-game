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

    // creating physics world
    b2WorldDef world_def = b2DefaultWorldDef();

    b2SetLengthUnitsPerMeter(TEXELS_PER_UNIT);
	world_def.gravity.y = GRAVITY_ACCELERATION * TEXELS_PER_UNIT;

	b2WorldId world_id = b2CreateWorld(&world_def);

    // creating physics ground
    b2BodyDef ground_def = b2DefaultBodyDef();

    ground_def.position = { 0.0f, -2.5f };

    b2BodyId ground_id = b2CreateBody(world_id, &ground_def);

    // creating shape for physics ground
    b2ShapeDef shape_def = b2DefaultShapeDef();

    b2Polygon polygon_box = b2MakeBox(10.0f, 2.0f);

    b2CreatePolygonShape(ground_id, &shape_def, &polygon_box);

    Camera3D camera;
    camera.position = { .0f, 2.5f, -5.0f };
    camera.target = { .0f, .0f, .0f };
    camera.up = { .0f, 1.0f, .0f };
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model dummy { LoadModel("../resources/monke.glb") };

    Player player { world_id };

    camera.target = Vector3Zeros;

    SetTargetFPS(512);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        b2World_Step(world_id, TIMESTEP, SUBSTEP_COUNT);

        player.animate();

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