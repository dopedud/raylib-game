#pragma once

#include <vector>

#include "raylib.h"

#include "box2d/box2d.h"

class ResourceManager
{
private:

    const b2WorldId m_world_id {};

    std::vector<Texture> m_player_idle {};

    const Shader m_player_shader {};

    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static const b2WorldId construct_world_id();

public:

    static ResourceManager& instance()
    {
        // created only once, thread-safe in C++11+
        static ResourceManager instance {};
        return instance;
    }

    const b2WorldId world_id() const { return m_world_id; }

    const std::vector<Texture>& player_idle() const { return m_player_idle; }

    const Shader player_shader() const { return m_player_shader; }
};