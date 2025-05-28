#include "player.h"

#include <array>

#include "raylib.h"
#include "raymath.h"

#include "box2d/box2d.h"

#include "settings.h"

Player::Player(b2WorldId world_id) :
model 
{ 
    8,
    true,
    "../resources/warrior/run/warrior_run",
    { .0f, .0f },
    "../resources/shaders/glsl/vertex.vs",
    "../resources/shaders/glsl/fragment.fs",
    .1f
}
{
    b2BodyDef bodydef = b2DefaultBodyDef();
    bodydef.position = { .0f, 2.5f };
    bodydef.type = b2_dynamicBody;
    m_bodyID = b2CreateBody(world_id, &bodydef);

    extent.x = model.width() / 2;
    extent.y = model.height() / 2;
    
    b2Polygon box = b2MakeBox(extent.x, extent.y);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.material.friction = 1.0f;

    b2CreatePolygonShape(m_bodyID, &shape_def, &box);

    b2MassData massdata {};
    massdata.mass = 1.0f;
    massdata.center = { .0f, .0f };
    massdata.rotationalInertia = .0f;

    b2Body_SetMassData(m_bodyID, massdata);
}

Player::~Player()
{
    model.~AnimatedModel();
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
    model.animate();
}

void Player::draw()
{
    b2Vec2 position = b2Body_GetPosition(m_bodyID);
    b2Rot rotation = b2Body_GetRotation(m_bodyID);
    float degrees = b2Rot_GetAngle(rotation) * RAD2DEG;

    DrawModelEx(model.model(), { position.x, position.y , .0f }, { .0f, .0f, 1.0f }, degrees, Vector3Ones, WHITE);


}