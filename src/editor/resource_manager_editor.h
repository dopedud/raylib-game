#pragma once

#include "raylib.h"
#include "imgui.h"

class ResourceManagerEditor
{
private:

    RenderTexture rt_game_view {};
    ImFont* font {};

    ResourceManagerEditor();
    ~ResourceManagerEditor();

    ResourceManagerEditor(const ResourceManagerEditor&) = delete;
    ResourceManagerEditor& operator=(const ResourceManagerEditor&) = delete;

public:

    inline static ResourceManagerEditor& instance()
    {
        // created only once, thread-safe in C++11+
        static ResourceManagerEditor instance {};
        return instance;
    }

    inline RenderTexture* game_view()
    {
        return &rt_game_view;
    }

    inline ImFont* font_resource()
    {
        return font;
    }

    RenderTexture* reload_game_view(ImVec2 viewport);
};