#pragma once

#include <array>

#include "raylib.h"

#include "animated_model.h"

#define IDLE_FRAMES 6

class Player
{
    AnimatedModel model;

public:
    Player();
    ~Player();

    void update();
    void draw();
};