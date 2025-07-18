/** 
 * @file
 * @brief Main source file to initiate the start of the game program.
 * 
 * Main source file that act as the main entry point for the game program to start.
 */

#include <iostream>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "box2d/box2d.h"

#include "settings.h"
#include "player.h"
#include "player_camera_controller.h"
#include "resource_manager.h"

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

/**
 * @fn
 * @brief Main entry function of the program. 
 * 
 * Any initialisation is done in here.
 * 
 * @param argc Argument count.
 * @param argv Given arguments in a string array.
 */
int main(int argc, char* argv[])
{
    settings::initialise();

    ResourceManager::instance();

    // creating physics ground
    b2BodyDef ground_def = b2DefaultBodyDef();

    ground_def.position = { .0f, -5.0f };

    b2BodyId ground_id = b2CreateBody(ResourceManager::instance().world_id(), &ground_def);

    // creating shape for physics ground
    b2ShapeDef shape_def = b2DefaultShapeDef();

    b2Polygon polygon_box = b2MakeBox(100.0f, 2.0f);

    b2CreatePolygonShape(ground_id, &shape_def, &polygon_box);

    PlayerCameraController playercam {};

    Player player {};

    Model dummy { LoadModel("../resources/monke.glb") };

    EnableCursor();
    SetTargetFPS(settings::GENERAL::TARGET_FPS);

    float physics_sim_count {};

    /*
    ** GAME SIMULATION UPDATE (UPDATES EVERY FRAME)
    */
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        /*
        ** PHYSICS SIMULATION UPDATE (UPDATES EVERY FIXED INTERVAL)
        */
        physics_sim_count += GetFrameTime();

        TraceLog(LOG_DEBUG, "%f, %f", b2Body_GetPosition(player.bodyID()).x, b2Body_GetPosition(player.bodyID()).y);

        while (physics_sim_count >= settings::PHYSICS::TIMESTEP)
        {
            b2World_Step(ResourceManager::instance().world_id(), settings::PHYSICS::TIMESTEP, settings::PHYSICS::SUBSTEP_COUNT);
            physics_sim_count -= settings::PHYSICS::TIMESTEP;
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
        ** USER INPUT HANDLING
        */
        player.handle_input();
        /*
        ** END USER INPUT HANDLING
        */

        /*
        ** DRAWING FUNCTIONS
        */
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(playercam.camera());
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