#pragma once

#include <array>

#include "raylib.h"

#include "box2d/box2d.h"

#include "animated_model.h"

#define IDLE_FRAMES 6

class Player
{
    AnimatedModel model;

    b2BodyId m_bodyID;
    Vector2 m_extent;

public:
    Player();
    ~Player();

    b2BodyId bodyID();
    Vector2 extent();

    void update();
    void draw();
};