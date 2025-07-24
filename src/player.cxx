#include "player.h"

#include <array>
#include <utility>

#include "raylib.h"
#include "raymath.h"

#include "box2d/box2d.h"

#include "settings.h"
#include "resource_manager.h"

Player::Player()
: state { PlayerState::PLAYER_IDLE }
, models
{
    AnimatedModel
    {
        TextureResource::PLAYER_IDLE,
        ShaderResource::PLAYER,
        ModelResource::PLAYER_IDLE,
        true,
        { .0f, .0f },
        .1f
    },
    
    AnimatedModel
    {
        TextureResource::PLAYER_RUN,
        ShaderResource::PLAYER,
        ModelResource::PLAYER_RUN,
        true,
        { .0f, .0f },
        .1f
    },

    AnimatedModel
    {
        TextureResource::PLAYER_SLIDE,
        ShaderResource::PLAYER,
        ModelResource::PLAYER_SLIDE,
        true,
        { .0f, .0f },
        .1f
    },
}
{
    bodyIDs.resize(models.size());

    for (size_t i = 0; i < models.size(); i++)
    {
        b2BodyDef bodydef = b2DefaultBodyDef();
        bodydef.type = b2_dynamicBody;
        bodydef.position = { .0f, 2.0f };
        bodyIDs[i] = b2CreateBody(ResourceManager::instance().world_id(), &bodydef);

        b2Vec2 extent {};
        extent.x = models[i].width() / 2;
        extent.y = models[i].height() / 2;

        b2Polygon box = b2MakeBox(extent.x, extent.y);
        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.material.friction = 1.0f;
        b2CreatePolygonShape(bodyIDs[i], &shape_def, &box);

        b2MassData massdata {};
        massdata.mass = 72.0f;
        massdata.center = { .0f, .0f };
        massdata.rotationalInertia = .0f;
        b2Body_SetMassData(bodyIDs[i], massdata);

        b2Body_Disable(bodyIDs[i]);
    }

    m_bodyID = bodyIDs[0];

    switch_to_body(bodyIDs[0]);

    m_model = &models[0];
}

Player::~Player()
{
    for (b2BodyId& bodyID : bodyIDs) b2DestroyBody(bodyID);
}

void Player::switch_to_body(b2BodyId newbody)
{
    b2Vec2 position = b2Body_GetPosition(m_bodyID);
    b2Rot rotation = b2Body_GetRotation(m_bodyID);
    bool fixed_rotation = b2Body_IsFixedRotation(m_bodyID);
    b2Vec2 velocity = b2Body_GetLinearVelocity(m_bodyID);
    float angular_velocity = b2Body_GetAngularVelocity(m_bodyID);
    
    b2Body_Disable(m_bodyID);

    b2Body_SetTransform(newbody, position, rotation);
    b2Body_SetFixedRotation(newbody, fixed_rotation);
    b2Body_SetLinearVelocity(newbody, velocity);
    b2Body_SetAngularVelocity(newbody, angular_velocity);
    b2Body_Enable(newbody);

    m_bodyID = newbody;
}

void Player::handle_input()
{
    if (!receive_input) return;

    if (IsKeyPressed(KEY_SPACE))
    {
        b2Body_ApplyForceToCenter(m_bodyID, { .0f, jump_force }, true);
    }

    if (IsKeyDown(KEY_A))
    {
        b2Body_ApplyForceToCenter(m_bodyID, { move_force, .0f }, true);
    }

    if (IsKeyDown(KEY_D))
    {
        b2Body_ApplyForceToCenter(m_bodyID, { -move_force, .0f }, true);
    }
}

void Player::animate()
{
    m_model->animate();
}

void Player::draw()
{
    b2Vec2 velocity = b2Body_GetLinearVelocity(m_bodyID);

    if (b2Length(velocity) > MAX_MOVE_VELOCITY)
    {
        velocity = b2Normalize(velocity);
        velocity *= MAX_MOVE_VELOCITY;
        b2Body_SetLinearVelocity(m_bodyID, velocity);
    }

    // TraceLog(LOG_DEBUG, "VELOCITY: %f", b2Length(velocity));

    b2Vec2 position = b2Body_GetPosition(m_bodyID);
    b2Rot rotation = b2Body_GetRotation(m_bodyID);
    float degrees = b2Rot_GetAngle(rotation) * RAD2DEG;

    // TraceLog(LOG_DEBUG, "%f, %f", position.x, position.y);

    switch (state.state())
    {
        case PlayerState::PLAYER_IDLE:
            DrawModelEx(*(m_model->model()),
            { position.x, position.y, .0f },
            { .0f, .0f, 1.0f }, degrees, Vector3Ones, WHITE);
        break;

        case PlayerState::PLAYER_MOVING:
        break;

        case PlayerState::PLAYER_JUMPING:
        break;
    }
}