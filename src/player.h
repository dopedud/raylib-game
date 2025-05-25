#pragma once

#include "box2d/box2d.h"

#include "animated_model.h"

#define IDLE_FRAMES 6

class Player
{
    AnimatedModel model;

    b2BodyId m_bodyID {};
    b2Vec2 m_extent {};

public:
    Player(b2WorldId world_id);
    ~Player();

    void animate();
    void draw();
};