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
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    m_bodyID = b2CreateBody(world_id, &body_def);

    m_extent.x = model.width() / 2;
    m_extent.y = model.height() / 2;
    
    b2Polygon box = b2MakeBox(m_extent.x, m_extent.y);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.material.friction = 0.5f;

    b2CreatePolygonShape(m_bodyID, &shape_def, &box);
}

Player::~Player()
{
    model.~AnimatedModel();
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