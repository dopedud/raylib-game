#include "player.h"

#include <array>

#include "raylib.h"
#include "raymath.h"

#include "settings.h"

Player::Player()
{
    for (int i = 0; i < IDLE_FRAMES; ++i)
    {
        textures_idle[i] = LoadTexture(TextFormat("../resources/warrior/idle/warrior_idle_%i.png", i));
        meshes_idle[i] = GenMeshCube(
            (float)textures_idle[i].width / -TEXELS_PER_UNIT,
            (float)textures_idle[i].height / -TEXELS_PER_UNIT,
            .0f);
    }

    model = LoadModelFromMesh(meshes_idle[0]);
    shader = LoadShader("../resources/shaders/glsl/vertex.vs", "../resources/shaders/glsl/fragment.fs");

    model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[0];
    model.materials[0].shader = shader;
}

Player::~Player()
{
    for (int i = 0; i < IDLE_FRAMES; ++i)
    {
        UnloadTexture(textures_idle[i]);
        UnloadMesh(meshes_idle[i]);
    }

    UnloadModel(model);
    UnloadShader(shader);
}

void Player::update()
{
    timer_idle += GetFrameTime();

    switch (frameindex_idle)
    {
        case 0:
            if (timer_idle > timing_idle[0])
            {
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[1];
                model.meshes[0] = meshes_idle[1];
                frameindex_idle++;
            }
        break;

        case 1:
            if (timer_idle > timing_idle[1])
            {
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[2];
                model.meshes[0] = meshes_idle[2];
                frameindex_idle++;
            }
        break;

        case 2:
            if (timer_idle > timing_idle[2])
            {
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[3];
                model.meshes[0] = meshes_idle[3];
                frameindex_idle++;
            }
        break;

        case 3:
            if (timer_idle > timing_idle[3])
            {
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[4];
                model.meshes[0] = meshes_idle[4];
                frameindex_idle++;
            }
        break;

        case 4:
            if (timer_idle > timing_idle[4])
            {
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[5];
                model.meshes[0] = meshes_idle[5];
                frameindex_idle++;
            }
        break;

        case 5:
            if (timer_idle > timing_idle[5])
            {
                model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures_idle[0];
                model.meshes[0] = meshes_idle[0];
                frameindex_idle = 0;
                timer_idle = 0;
            }
        break;
    }
}

void Player::draw()
{
    DrawModel(model, Vector3Zeros, 1.0f, WHITE);
}