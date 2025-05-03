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

float TEXELS_PER_UNIT { 16.0f };

int main(int argc, char* argv[])
{
    initialise();

    Camera3D camera;
    camera.position = { .0f, 1.0f, -5.0f };
    camera.target = { .0f, .0f, .0f };
    camera.up = { .0f, 1.0f, .0f };
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model dummy { LoadModel("../resources/monke.glb") };

    Image player_sprite { LoadImage("../resources/warrior/idle/Warrior_Idle_1.png") };
    ImageCrop(&player_sprite, Rectangle{ 18, 10, 18, 33 } );
    ImageRotateCW(&player_sprite);
    ImageRotateCW(&player_sprite);

    Texture player_texture { LoadTextureFromImage(player_sprite) };

    Vector3 player_position { .0f, .0f, .0f };
    Model player { LoadModelFromMesh(GenMeshCube((float)player_texture.width / TEXELS_PER_UNIT, (float)player_texture.height / TEXELS_PER_UNIT, .0f)) };
    // Texture player_texture { LoadTextureFromImage(GenImageCellular(512, 512, 50)) };
    Shader player_shader { LoadShader("../resources/shaders/glsl/vertex.vs", "../resources/shaders/glsl/fragment.fs") };

    int shaderloc_pixels_per_unit { GetShaderLocation(player_shader, "texels_per_unit") };
    int shaderloc_texture_size { GetShaderLocation(player_shader, "texture_size") };

    player.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = player_texture;
    player.materials[0].shader = player_shader;

    Vector2 player_texture_size { (float)player_texture.width, (float)player_texture.height };
    SetShaderValue(player_shader, shaderloc_pixels_per_unit, &TEXELS_PER_UNIT, SHADER_UNIFORM_FLOAT);
    SetShaderValue(player_shader, shaderloc_texture_size, &player_texture_size, SHADER_UNIFORM_VEC2);

    camera.target = player_position;

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        if (IsKeyPressed(KEY_UP)) 
        player.transform = MatrixScale(player.transform.m0 + 1.0f,
                                       player.transform.m5,
                                       player.transform.m10);
        if (IsKeyPressed(KEY_DOWN) && TEXELS_PER_UNIT > 8.0f)
        player.transform = MatrixScale(player.transform.m0 - 1.0f,
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