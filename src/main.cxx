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
#include "player_camera_controller.h"

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

    ground_def.position = { .0f, -2.0f };

    b2BodyId ground_id = b2CreateBody(world_id, &ground_def);

    // creating shape for physics ground
    b2ShapeDef shape_def = b2DefaultShapeDef();

    b2Polygon polygon_box = b2MakeBox(100.0f, 2.0f);

    b2CreatePolygonShape(ground_id, &shape_def, &polygon_box);

    PlayerCameraController player_camera_controller {};

    Model dummy { LoadModel("../resources/monke.glb") };

    Player player { world_id };

    EnableCursor();
    SetTargetFPS(TARGET_FPS);

    float physics_sim_count {};

    /*
    ** GAME SIMULATION UPDATE (UPDATE EVERY FRAME)
    */
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        /*
        ** PHYSICS SIMULATION UPDATE (UPDATE EVERY FIXED INTERVAL)
        */
        physics_sim_count += GetFrameTime();

        while (physics_sim_count >= TIMESTEP)
        {
            b2World_Step(world_id, TIMESTEP, SUBSTEP_COUNT);

            physics_sim_count -= TIMESTEP;
        }
        /*
        ** END PHYSICS SIMULATION
        */

        /*
        ** ANIMATION SIMULATION
        */
        player.animate();
        /*
        ** END ANIMATION SIMULATION
        */

        /*
        ** RECEIVE USER INPUT
        */
        player.handle_input();
        /*
        ** END RECEIVE USER INPUT
        */

        /*
        ** CAMERA MOVEMENT
        */
        
        /*
        ** PLAYER CAMERA MOVEMENT
        */

        /*
        ** DRAWING FUNCTIONS
        */
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(player_camera_controller.camera());
                DrawGrid(100, 1);
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                player.draw();
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
        /*
        ** END DRAWING FUNCTIONS
        */
    }

    UnloadModel(dummy);

    CloseWindow();

    return 0;
}