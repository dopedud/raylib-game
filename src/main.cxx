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
#include "rlgl.h"

#include "box2d/box2d.h"

#include "settings.h"
#include "player.h"
#include "player_camera_controller.h"
#include "resource_manager.h"

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

void DrawGridY(int slices, float spacing);

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

    Vector2 ground_size { 200.0f, .25f };
    Vector2 ground_position { .0f, -2.0f };

    // defining physics body for ground
    b2BodyDef ground_def = b2DefaultBodyDef();
    ground_def.position = { ground_position.x, ground_position.y };
    b2BodyId ground_id = b2CreateBody(ResourceManager::instance().world_id(), &ground_def);

    // creating physics shape for ground
    b2ShapeDef shape_def = b2DefaultShapeDef();
    b2Polygon polygon_box = b2MakeBox(ground_size.x / 2, ground_size.y / 2);
    b2CreatePolygonShape(ground_id, &shape_def, &polygon_box);

    // generating mesh and material for ground
    Image imageGen = GenImageColor(1024, 1024, WHITE);
    Model ground_model = LoadModelFromMesh(GenMeshCube(-ground_size.x, -ground_size.y, .0f));
    ground_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(imageGen);
    UnloadImage(imageGen);

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
                DrawGridY(100, 1);
                DrawGrid(100, 1);
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                DrawModelEx(ground_model, { ground_position.x, ground_position.y, .0f }, 
                { .0f, .0f, 1.0f }, .0f, Vector3Ones, DARKGRAY);
                player.draw();
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
        /*
        ** END DRAWING FUNCTIONS
        */
    }

    b2DestroyBody(ground_id);
    UnloadModel(ground_model);

    UnloadModel(dummy);

    CloseWindow();

    return 0;
}

void DrawGridY(int slices, float spacing)
{
    int halfSlices = slices/2;

    rlBegin(RL_LINES);
        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            if (i == 0)
            {
                rlColor3f(0.5f, 0.5f, 0.5f);
            }
            else
            {
                rlColor3f(0.75f, 0.75f, 0.75f);
            }

            rlVertex3f((float)i*spacing, (float)-halfSlices*spacing, 0.0f);
            rlVertex3f((float)i*spacing, (float)halfSlices*spacing, 0.0f);

            rlVertex3f((float)-halfSlices*spacing, (float)i*spacing, 0.0f);
            rlVertex3f((float)halfSlices*spacing, (float)i*spacing, 0.0f);
        }
    rlEnd();
}