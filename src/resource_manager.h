#pragma once

#include "raylib.h"

#include "box2d/box2d.h"

class ResourceManager
{
private:

    b2WorldId m_world_id {};

    

    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:

    static ResourceManager& instance()
    {
        // created only once, thread-safe in C++11+
        static ResourceManager instance {};
        return instance;
    }

    b2WorldId world_id() const
    {
        return m_world_id;
    }
};