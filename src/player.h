#pragma once

#include <array>

#include "raylib.h"

#include "box2d/box2d.h"

#include "animated_model.h"

#define IDLE_FRAMES 6

class Player
{
    AnimatedModel model;

    b2BodyId bodyID;
    Vector2 extent;

public:
    Player();
    ~Player();

    void update();
    void draw();
};