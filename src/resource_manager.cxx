#include "resource_manager.h"

#include "box2d/box2d.h"

#include "settings.h"

ResourceManager::ResourceManager()
{
    // creating physics world
    b2WorldDef world_def { b2DefaultWorldDef() };

    b2SetLengthUnitsPerMeter(TEXELS_PER_UNIT);
    world_def.gravity.y = GRAVITY_ACCELERATION * TEXELS_PER_UNIT;

    m_world_id = b2CreateWorld(&world_def);
}

ResourceManager::~ResourceManager()
{
    
}