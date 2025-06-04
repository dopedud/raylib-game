#pragma once

#include <vector>

#include "box2d/box2d.h"

#include "animated_model.h"
#include "state_manager.h"

#define IDLE_FRAMES 6

#define JUMP_FORCE_MULTIPLIER 500.0f

typedef enum 
{
    PLAYER_IDLE,
    PLAYER_MOVING,
    PLAYER_SLIDE,
    PLAYER_JUMPING,
} PlayerState;

class Player
{
private:
    StateManager<PlayerState> state {};

    b2BodyId m_bodyID {};
    std::vector<b2BodyId> bodyIDs {};

    const float MAX_MOVE_VELOCITY { 10.0f };
    float move_force { 10.0f };
    float jump_force { 2.5f };

    std::vector<AnimatedModel> models;

    bool facingleft {};

    bool receive_input { true };

    void switch_to_body(b2BodyId newbody);

public:
    Player(b2WorldId world_id);
    ~Player();

    b2BodyId bodyID() const { return m_bodyID; }

    void set_input(bool toggle) { receive_input = toggle; }

    void handle_input();
    void animate();
    void draw();
};