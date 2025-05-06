#pragma once

#include <array>

#include "raylib.h"

#define IDLE_FRAMES 6

class Player
{
    Model model {};
    Shader shader {};

    std::array<Texture, IDLE_FRAMES> textures_idle {};
    std::array<Mesh, IDLE_FRAMES> meshes_idle {};
    int frameindex_idle {};
    float timer_idle {};
    std::array<float, IDLE_FRAMES> timing_idle {
        .1f,
        .2f,
        .3f,
        .4f,
        .5f,
        .6f
    };

public:
    Player();
    ~Player();

    void update();
    void draw();
};