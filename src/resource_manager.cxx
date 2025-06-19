#include "resource_manager.h"

#include <string>

#include "box2d/box2d.h"

#include "settings.h"

#define FRAMES_AMOUNT_PLAYER_IDLE 6

ResourceManager::ResourceManager()
: m_world_id { construct_world_id() }
, m_player_shader { LoadShader( 
    "../resources/shaders/glsl/vertex.vs", 
    "../resources/shaders/glsl/fragment.fs") }
{
    m_player_idle.resize(FRAMES_AMOUNT_PLAYER_IDLE);

    for (int i = 0; i < FRAMES_AMOUNT_PLAYER_IDLE; i++)
    {
        std::string path { "../resources/warrior/idle/warrior_idle_" + std::to_string(i) + ".png" };
        m_player_idle[i] = LoadTexture(path.c_str());
    }
}

ResourceManager::~ResourceManager()
{
    for (auto texture : m_player_idle) UnloadTexture(texture);

    UnloadShader(m_player_shader);
}

const b2WorldId ResourceManager::construct_world_id()
{
    b2WorldDef world_def { b2DefaultWorldDef() };

    b2SetLengthUnitsPerMeter(TEXELS_PER_UNIT);
    world_def.gravity.y = GRAVITY_ACCELERATION * TEXELS_PER_UNIT;

    return b2CreateWorld(&world_def);
}