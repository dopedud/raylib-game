#pragma once

#include "box2d/box2d.h"

#include "animated_model.h"

#define IDLE_FRAMES 6

class Player
{
    AnimatedModel model;

    b2BodyId m_bodyID {};
    b2Vec2 extent {};

    const float MAX_MOVE_VELOCITY { 10.0f };
    float move_force { 10.0f };
    float jump_force { 500.0f };

    bool receive_input { true };

    bool debug_draw { true };

public:
    Player(b2WorldId world_id);
    ~Player();

    b2BodyId bodyID() { return m_bodyID; }

    void set_input(bool toggle) { receive_input = toggle; }

    void handle_input();
    void animate();
    void draw();
};