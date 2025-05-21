#include "player.h"

#include <array>

#include "raylib.h"
#include "raymath.h"

#include "settings.h"

Player::Player() : model 
{ 
    8,
    true,
    "../resources/warrior/run/warrior_run",
    "../resources/shaders/glsl/vertex.vs",
    "../resources/shaders/glsl/fragment.fs",
    .1f
}
{}

Player::~Player()
{
    model.~AnimatedModel();
}

void Player::update()
{
    model.update();
}

void Player::draw()
{
    DrawModel(model.model(), Vector3Zeros, 1.0f, WHITE);
}