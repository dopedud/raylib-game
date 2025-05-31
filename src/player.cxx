#include "player.h"

#include <array>

#include "raylib.h"
#include "raymath.h"

#include "box2d/box2d.h"

#include "settings.h"

Player::Player(b2WorldId world_id)
{
    models.push_back
    (
        AnimatedModel
        {
            8,
            true,
            "../resources/warrior/run/warrior_run",
            { .0f, .0f },
            "../resources/shaders/glsl/vertex.vs",
            "../resources/shaders/glsl/fragment.fs",
            .05f
        }
    );

    models.push_back
    (
        AnimatedModel
        {
            8,
            true,
            "../resources/warrior/run/warrior_run",
            { .0f, .0f },
            "../resources/shaders/glsl/vertex.vs",
            "../resources/shaders/glsl/fragment.fs",
            .05f
        }
    );

    bodyIDs.resize(models.size());

    TraceLog(LOG_DEBUG, "reached here 1");

    for (int i = 0; i < models.size(); i++)
    {
        b2BodyDef bodydef = b2DefaultBodyDef();
        bodydef.type = b2_dynamicBody;
        bodyIDs[i] = b2CreateBody(world_id, &bodydef);

        TraceLog(LOG_DEBUG, "reached here 2");

        b2Vec2 extent {};

        extent.x = models[i].width() / 2;
        extent.y = models[i].height() / 2;

        b2Polygon box = b2MakeBox(extent.x, extent.y);

        TraceLog(LOG_DEBUG, "reached here 3");

        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.material.friction = 1.0f;

        b2CreatePolygonShape(bodyIDs[i], &shape_def, &box);

        TraceLog(LOG_DEBUG, "reached here 4");

        b2MassData massdata {};
        massdata.mass = 1.0f;
        massdata.center = { .0f, .0f };
        massdata.rotationalInertia = .0f;

        b2Body_SetMassData(bodyIDs[i], massdata);

        b2Body_Disable(bodyIDs[i]);
    }

    m_bodyID = bodyIDs[0];

    switch_to_body(bodyIDs[0]);
}

Player::~Player()
{
    for (const AnimatedModel& model : models)
    model.~AnimatedModel();
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
        b2Body_ApplyForceToCenter(m_bodyID, { .0f, jump_force * JUMP_FORCE_MULTIPLIER }, true);
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
    switch (state.state())
    {
        case PLAYER_IDLE:
            models[0].animate();
        break;

        case PLAYER_MOVING:
        break;

        case PLAYER_JUMPING:
        break;
    }
}

void Player::draw()
{
    b2Vec2 position = b2Body_GetPosition(m_bodyID);
    b2Rot rotation = b2Body_GetRotation(m_bodyID);
    float degrees = b2Rot_GetAngle(rotation) * RAD2DEG;

    switch (state.state())
    {
        case PLAYER_IDLE:
        //     DrawModelEx(model.model(),
        //     { position.x, position.y , .0f },
        //     { .0f, .0f, 1.0f }, degrees, Vector3Ones, WHITE);
        break;

        case PLAYER_MOVING:
        break;

        case PLAYER_JUMPING:
        break;
    }
}