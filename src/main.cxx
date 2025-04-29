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

float PIXELS_PER_UNIT { 64.0f };

int main(int argc, char* argv[])
{
    initialise();

    Camera3D camera;
    camera.position = { .0f, 1.0f, -8.0f };
    camera.target = { camera.position.x, camera.position.y , camera.position.z + 1.0f };
    // camera.target = { .0f, .0f, .0f };
    camera.up = { .0f, 1.0f, .0f };
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model dummy { LoadModel("../resources/monke.glb") };

    Image player_sprite { LoadImage("../resources/Warrior/Individual Sprite/idle/Warrior_Idle_1.png") };
    ImageRotateCW(&player_sprite);
    ImageRotateCW(&player_sprite);

    Vector3 player_position { .0f, 2.0f, .0f };
    Vector2 plane_size { 2.0f, 2.0f };
    Model player { LoadModelFromMesh(GenMeshCube(plane_size.x, plane_size.y, .0f)) };
    Texture player_texture { LoadTextureFromImage(player_sprite) };
    Shader player_shader { LoadShader("../resources/shaders/glsl/vertex.vs", "../resources/shaders/glsl/fragment.fs") };

    int shaderloc_pixels_per_unit { GetShaderLocation(player_shader, "pixels_per_unit") };
    int shaderloc_texture_size { GetShaderLocation(player_shader, "texture_size") };

    player.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = player_texture;
    player.materials[0].shader = player_shader;

    Vector2 player_texture_size { (float)player_texture.width, (float)player_texture.height };
    SetShaderValue(player_shader, shaderloc_texture_size, &player_texture_size, SHADER_UNIFORM_VEC2);
    SetShaderValue(player_shader, shaderloc_pixels_per_unit, &PIXELS_PER_UNIT, SHADER_UNIFORM_FLOAT);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        if (IsKeyDown(KEY_UP)) player.transform = MatrixScale(player.transform.m0 + 1.0f, 
                                                              player.transform.m5,
                                                              player.transform.m10);
        if (IsKeyDown(KEY_DOWN) && PIXELS_PER_UNIT > 8.0f) player.transform = MatrixScale(player.transform.m0 - 1.0f,
                                                                                          player.transform.m5,
                                                                                          player.transform.m10);

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);
                DrawGrid(25, 1.0f);
                DrawModel(dummy, { .0f, .0f, 10.0f }, 1.0f, WHITE);
                DrawModel(player, player_position, 1.0f, WHITE);
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(dummy);
    UnloadModel(player);

    UnloadTexture(player_texture);

    UnloadShader(player_shader);

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