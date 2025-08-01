/** 
 * @file
 * @brief Main source file to initiate the start of the game program.
 * 
 * Main source file that act as the main entry point for the game program to start.
 */

#define DEBUG ///// NOTE: TEMPORARY

#include <iostream>

#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "rlImGui.h"

#include "box2d/box2d.h"

#include "settings.h"
#include "player.h"
#include "camera_controller.h"
#include "resource_manager.h"

#ifdef DEBUG
#include "settings_editor.h"
#include "resource_manager_editor.h"
#endif

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
#ifdef DEBUG
    settings_editor::initialise();
    ResourceManagerEditor::instance();
#endif

    Vector2 ground_size { 200.0f, .25f };
    Vector2 ground_position { .0f, -2.0f };

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
                DrawModelEx(ground_model, { ground_position.x, ground_position.y, .0f }, 
                { .0f, .0f, 1.0f }, .0f, Vector3Ones, DARKGRAY);
                player.draw();
            EndMode3D();
        EndTextureMode();
#endif

        BeginDrawing();
            ClearBackground(Color{253, 246, 227, 255});
#ifndef DEBUG
            /**
             * DRAW WORLD
             */
            BeginMode3D(camera.camera());
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
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
#else
            /**
             * DRAW EDITOR
             */
            rlImGuiBegin();
            ImGui::PushFont(ResourceManagerEditor::instance()->font_resource(), 18.0f);

            ImGuiViewport* viewport { ImGui::GetMainViewport() };
            ImGui::SetNextWindowViewport(viewport->ID);
            ImVec2 WorkPos { viewport->WorkPos };
            ImVec2 WorkSize { viewport->WorkSize };

            ImGui::SetNextWindowPos(WorkPos);
            ImGui::SetNextWindowSize(WorkSize);
            ImGui::Begin("Main", nullptr, 
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |

                ImGuiWindowFlags_MenuBar
            );
                ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(), ImGuiDockNodeFlags_None);

                static bool show_demo {};
                static bool show_metrics {};

                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("Settings"))
                    {
                        ImGui::MenuItem("Show Demo Window", nullptr, &show_demo);
                        ImGui::MenuItem("Show Metrics Window", nullptr, &show_metrics);
                        ImGui::EndMenu();
                    }

                    ImGui::MenuItem("About");

                    ImGui::EndMenuBar();
                }
            ImGui::End();

            if (show_demo) ImGui::ShowDemoWindow();
            if (show_metrics) ImGui::ShowMetricsWindow();

            // SETUP DOCK WINDOWS ONCE
            static bool init_dock { true };
            if (init_dock)
            {
                init_dock = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, WorkSize);

                ImGuiID left {};
                ImGuiID right {};

                ImGui::DockBuilderSplitNode(
                    dockspace_id,
                    ImGuiDir_Left,
                    .3f,
                    &left,
                    &right
                );

                ImGui::DockBuilderDockWindow("Variables", left);
                ImGui::DockBuilderDockWindow("Viewport", right);
                ImGui::DockBuilderFinish(dockspace_id);
            }

            ImGui::Begin("Variables", nullptr, ImGuiWindowFlags_NoCollapse);

                ImGui::TextWrapped("Camera Max Speed");

                float max_speed { camera.max_speed() };
                ImGui::SliderFloat("##camera_max_speed",
                    &max_speed,
                    CameraController::MAX_SPEED_MIN,
                    CameraController::MAX_SPEED_MAX,
                    "%.0f",
                    ImGuiSliderFlags_AlwaysClamp
                );
                camera.set_max_speed(max_speed);

                ImGui::TextWrapped("Camera Smooth Multiplier");

                float smooth_multiplier { camera.smooth_multiplier() };
                ImGui::SliderFloat("##camera_smooth_multiplier",
                    &smooth_multiplier,
                    CameraController::SMOOTH_MULTIPLIER_MIN,
                    CameraController::SMOOTH_MULTIPLIER_MAX,
                    "%.0f",
                    ImGuiSliderFlags_AlwaysClamp
                );
                camera.set_smooth_multiplier(smooth_multiplier);
            ImGui::End();
                
            ImGui::Begin("Viewport", nullptr, 
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_MenuBar |
                ImGuiWindowFlags_HorizontalScrollbar
            );
                if (ImGui::BeginMenuBar())
                {
                    ImGui::MenuItem("Setings");
                    ImGui::EndMenuBar();
                }

                ImVec2 content_region { ImGui::GetContentRegionAvail() };

                RenderTexture* game_view { ResourceManagerEditor::instance()->game_view() };

                if ((float)game_view->texture.width != content_region.x ||
                (float)game_view->texture.height != content_region.y)
                game_view = ResourceManagerEditor::instance()->reload_game_view(content_region);

                rlImGuiImageRenderTexture(game_view);
            ImGui::End();

            ImGui::PopFont();
            rlImGuiEnd();
            /**
             * END DRAW EDITOR
             */
#endif
        EndDrawing();
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
#ifdef DEBUG
    settings_editor::deinitialise();
#endif

    return 0;
}