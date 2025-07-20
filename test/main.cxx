#include <iostream>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "box2d/box2d.h"

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

constexpr int SCREEN_WIDTH { 1280 };
constexpr int SCREEN_HEIGHT { 720 };

void initialise();

int main()
{
    constexpr float PHYSICS_TIMESTEP { 1.0f / 60.0f };
    constexpr int PHYSICS_SUBSTEP_COUNT { 4 };

    initialise();

    b2WorldDef world_def { b2DefaultWorldDef() };
    b2SetLengthUnitsPerMeter(40.0f);
    world_def.gravity.y = -9.81f * 40.0f;
    b2WorldId world_id = b2CreateWorld(&world_def);

    Vector2 ground_size { 2000.0f,  40.0f };
    b2Vec2 ground_position { (float)SCREEN_WIDTH / 2, -(float)SCREEN_HEIGHT };

    Vector2 player_size { 40.0f, 40.0f };
    b2Vec2 player_initial_position { (float)SCREEN_WIDTH / 2, -(float)SCREEN_HEIGHT + 10.0f };

    // defining physics body for ground
    b2BodyDef ground_body = b2DefaultBodyDef();
    ground_body.position = ground_position;
    b2BodyId ground_id = b2CreateBody(world_id, &ground_body);

    // creating physics shape for ground
    b2ShapeDef ground_shape = b2DefaultShapeDef();
    b2Polygon ground_box = b2MakeBox(ground_size.x / 2, ground_size.y / 2);
    b2CreatePolygonShape(ground_id, &ground_shape, &ground_box);

    // defining physics body for player
    b2BodyDef player_body = b2DefaultBodyDef();
    player_body.type = b2_dynamicBody;
    player_body.position = player_initial_position;
    b2BodyId player_id = b2CreateBody(world_id, &player_body);

    // creating physics shape for player
    b2Polygon player_box = b2MakeBox(player_size.x / 2, player_size.y / 2);
    b2ShapeDef player_shape = b2DefaultShapeDef();
    player_shape.material.friction = 1.0f;
    b2CreatePolygonShape(player_id, &player_shape, &player_box);

    // setting mass data for player
    b2MassData massdata {};
    massdata.mass = 72.0f;
    massdata.center = { .0f, .0f };
    massdata.rotationalInertia = .0f;
    b2Body_SetMassData(player_id, massdata);

    Camera2D camera;
    camera.zoom = 1.0f;
    camera.target = { .0f, .0f };

    SetTargetFPS(512);

    EnableCursor();

    float physics_sim_count {};

    while(!WindowShouldClose())
    {
        physics_sim_count += GetFrameTime();

        while (physics_sim_count >= PHYSICS_TIMESTEP)
        {
            TraceLog(LOG_DEBUG, "ITERATING PHYSICS STEP...");
            b2World_Step(world_id, PHYSICS_TIMESTEP, PHYSICS_SUBSTEP_COUNT);
            physics_sim_count -= PHYSICS_TIMESTEP;
        }

        b2Vec2 player_position = b2Body_GetPosition(player_id);
        b2Rot player_rotation = b2Body_GetRotation(player_id);
        float degrees = b2Rot_GetAngle(player_rotation) * RAD2DEG;

        TraceLog(LOG_DEBUG, "%f, %f", player_position.x, player_position.y);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                DrawRectanglePro(
                    { player_position.x, -player_position.y, player_size.x, player_size.y },
                    { player_size.x / 2, player_size.y / 2 },
                    degrees, RED
                );

                DrawRectanglePro(
                    { ground_position.x, -ground_position.y, ground_size.x, ground_size.y },
                    { ground_size.x / 2, ground_size.y / 2 },
                    .0f,
                    DARKGRAY
                );

            EndMode2D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    b2DestroyWorld(world_id);

    CloseWindow();

    return 0;
}

void initialise()
{
    SetTraceLogLevel(LOG_DEBUG);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TEST WINDOW");
}