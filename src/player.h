#pragma once

#include <vector>
#include <memory>

#include "box2d/box2d.h"

#include "animated_model.h"
#include "state_manager.h"

enum class PlayerState 
{
    PLAYER_IDLE,
    PLAYER_MOVING,
    PLAYER_SLIDE,
    PLAYER_JUMPING,
};

class Player
{
private:

    StateManager<PlayerState> state {};

    b2BodyId m_bodyID {};
    std::vector<b2BodyId> bodyIDs {};

    static constexpr float MAX_MOVE_VELOCITY { 10.0f };
    float move_force { 1000.0f };
    float jump_force { 10000.0f };

    std::unique_ptr<AnimatedModel> m_model {};
    std::vector<AnimatedModel> models {};

    bool facingleft {};

    bool receive_input { true };

    void switch_to_body(b2BodyId newbody);

public:

    Player();
    ~Player();

    inline b2BodyId bodyID() const { return m_bodyID; }
    inline AnimatedModel* model() const { return m_model.get(); }

    inline void set_input(bool toggle) { receive_input = toggle; }

    void handle_input();
    void animate();
    void draw();
};