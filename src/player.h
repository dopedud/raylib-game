#pragma once

#include <vector>

#include "box2d/box2d.h"

#include "animated_model.h"
#include "state_manager.h"

#define IDLE_FRAMES 6

#define JUMP_FORCE_CONSTANT 500.0f

typedef enum 
{
    PLAYER_IDLE,
    PLAYER_MOVING,
    PLAYER_SLIDE,
    PLAYER_JUMPING,
} PlayerState;

class Player
{
    StateManager<PlayerState> state {};

    b2BodyId m_bodyID {};
    b2Vec2 extent {};

    const float MAX_MOVE_VELOCITY { 10.0f };
    float move_force { 10.0f };
    float jump_force { 2.5f };

    // AnimatedModel model;
    bool facingleft {};

    std::vector<AnimatedModel> models;

    bool receive_input { true };

public:
    Player(b2WorldId world_id);
    ~Player();

    b2BodyId bodyID() const { return m_bodyID; }

    void set_input(bool toggle) { receive_input = toggle; }

    void handle_input();
    void animate();
    void draw();
};