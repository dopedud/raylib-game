/** 
 * @file
 * @brief Main source file to initiate the start of the game program.
 * 
 * Main source file that act as the main entry point for the game program to start.
 */

///// NOTE: TEMPORARY
#define DEBUG

#include "raylib.h"
#include "raymath.h"

#include "box2d/box2d.h"

#include "settings.h"
#include "player.h"
#include "camera_controller.h"
#include "resource_manager.h"

#ifdef DEBUG
#include "editor.h"
#include "resource_manager_editor.h"
#include "settings_editor.h"
#endif

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

#ifdef DEBUG
    Editor::instance();
    bool show_editor { true };
#endif

    Vector2 ground_size { 200.0f, .25f };
    Vector2 ground_position { .0f, .0f };

    // defining physics body for ground
    b2BodyDef ground_def { b2DefaultBodyDef() };
    ground_def.position = { ground_position.x, ground_position.y };
    b2BodyId ground_id { b2CreateBody(ResourceManager::instance()->world_id(), &ground_def) };

    // creating physics shape for ground
    b2ShapeDef shape_def { b2DefaultShapeDef() };
    b2Polygon polygon_box { b2MakeBox(ground_size.x / 2, ground_size.y / 2) };
    b2CreatePolygonShape(ground_id, &shape_def, &polygon_box);

    // generating mesh and material for ground
    Image imageGen { GenImageColor(1024, 1024, WHITE) };
    Model ground_model { LoadModelFromMesh(GenMeshCube(-ground_size.x, -ground_size.y, .0f)) };
    ground_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(imageGen);
    UnloadImage(imageGen);

    CameraController camera {};

    Player player {};

    Model dummy { LoadModel("../resources/monke.glb") };
    Model land { LoadModel("../resources/land.glb") };

    EnableCursor();

    float physics_sim_count {};
    float input_poll_count {};

    /**
     * GAME SIMULATION UPDATE (UPDATES EVERY FRAME)
     */
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        /**
         * PHYSICS SIMULATION UPDATE (UPDATES EVERY FIXED INTERVAL)
         */
        physics_sim_count += GetFrameTime();

        while (physics_sim_count >= settings::PHYSICS::TIMESTEP)
        {
            b2World_Step(ResourceManager::instance()->world_id(), settings::PHYSICS::TIMESTEP, settings::PHYSICS::SUBSTEP_COUNT);
            
            physics_sim_count -= settings::PHYSICS::TIMESTEP;
        }
        /**
         * END PHYSICS SIMULATION UPDATE
         */
        
        /**
         * INPUT POLLING UPDATE (UPDATES EVERY FIXED HIGH FREQUENCY INTERVAL)
         */
        input_poll_count += GetFrameTime();

        while (input_poll_count >= settings::INPUT::TIMESTEP)
        {
            camera.handle_input();
            // player.handle_input();

            input_poll_count -= settings::INPUT::TIMESTEP;
        }
        /**
         * END INPUT POLLING UPDATE
         */

        /**
         * ANIMATION SIMULATION
         */
        player.animate();
        /**
         * END ANIMATION SIMULATION
         */

        /**
         * DRAWING FUNCTIONS
         */
#ifdef DEBUG
        BeginTextureMode(*ResourceManagerEditor::instance()->game_view());
            ClearBackground(Color{253, 246, 227, 255});
            BeginMode3D(camera.camera());
                DrawGrid(100, 1);
                settings_editor::DrawGridY(100, 1);
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                DrawModel(land, { .0f, .0f, .0f }, 1.0f, WHITE);
                DrawModelEx(ground_model, { ground_position.x, ground_position.y, .0f },
                { .0f, .0f, 1.0f }, .0f, Vector3Ones, DARKGRAY);
                player.draw();
            EndMode3D();
        EndTextureMode();

        if (IsKeyPressed(KEY_F5)) { show_editor = !show_editor; }

        if (!show_editor)
        {
#endif
            BeginDrawing();
                ClearBackground(Color{253, 246, 227, 255});

                /**
                 * DRAW WORLD
                 */
                BeginMode3D(camera.camera());
                    DrawGrid(100, 1);
                    settings_editor::DrawGridY(100, 1);
                    DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                    DrawModel(land, { .0f, .0f, .0f }, 1.0f, WHITE);
                    DrawModelEx(ground_model, { ground_position.x, ground_position.y, .0f },
                    { .0f, .0f, 1.0f }, .0f, Vector3Ones, DARKGRAY);
                    player.draw();
                EndMode3D();
                /**
                 * END DRAW WORLD
                 */

                /**
                 * DRAW UI
                 */

                /**
                 * END DRAW UI
                 */

            EndDrawing();

#ifdef DEBUG
        }

        else 
        {   
            BeginDrawing();
                /**
                 * DRAW EDITOR
                 */
                Editor::instance()->draw();
                /**
                 * END DRAW EDITOR
                 */
            EndDrawing();
        }
#endif
        /**
         * END DRAWING FUNCTIONS
         */
    }
    /**
     * END GAME SIMULATION UPDATE
     */

    b2DestroyBody(ground_id);
    UnloadModel(ground_model);

    UnloadModel(dummy);

    settings::deinitialise();
    ResourceManager::destroy();

#ifdef DEBUG
    Editor::destroy();
#endif

    return 0;
}