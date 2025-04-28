__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include <iostream>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "box2d/box2d.h"

#include "logger.h"

void initialise();

constexpr int SCREEN_WIDTH { 1280 };
constexpr int SCREEN_HEIGHT { 720 };

int main(int argc, char* argv[])
{
    initialise();

    // Define the camera to look into our 3d world
    Camera3D camera;
    camera.position = { .0f, 1.0f, -8.0f };
    // camera.target = { camera.position.x, camera.position.y , camera.position.z + 1.0f };
    camera.target = { .0f, .0f, .0f };
    camera.up = { .0f, 1.0f, .0f };
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model dummy = LoadModel("../resources/monke.glb");

    Mesh player_mesh = GenMeshCube(1.0f, 1.0f, .0f);
    Model player = LoadModelFromMesh(player_mesh);

    Texture player_texture = LoadTexture("../resources/Warrior/Individual Sprite/idle/Warrior_Idle_1.png");
    SetMaterialTexture(player.materials, MATERIAL_MAP_ALBEDO, player_texture);

    player.transform = MatrixRotateZ(PI);

    TraceLog(LOG_DEBUG, "1st row: %f %f %f %f", player.transform.m0, player.transform.m4,
                                                player.transform.m8, player.transform.m12);
    TraceLog(LOG_DEBUG, "2nd row: %f %f %f %f", player.transform.m1, player.transform.m5,
                                                player.transform.m9, player.transform.m13);
    TraceLog(LOG_DEBUG, "3rd row: %f %f %f %f", player.transform.m2, player.transform.m6,
                                                player.transform.m10, player.transform.m14);
    TraceLog(LOG_DEBUG, "4th row: %f %f %f %f", player.transform.m3, player.transform.m7,
                                                player.transform.m11, player.transform.m15);

    TraceLog(LOG_DEBUG,  "texture dimension: %i x %i", player_texture.width, player_texture.height);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);
                DrawGrid(25, 1.0f);
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                DrawModel(player, Vector3Zeros, 1.0f, WHITE);
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(dummy);
    UnloadModel(player);

    UnloadTexture(player_texture);

    CloseWindow();

    return 0;
}

void initialise()
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogLevel(LOG_DEBUG);
    SetTraceLogCallback(TimestampLogCallback);

    SetRandomSeed(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    SetExitKey(KEY_NULL);
}