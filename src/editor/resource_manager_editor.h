#pragma once

#include <memory>
#include <mutex>

#include "raylib.h"
#include "imgui.h"

class ResourceManagerEditor
{
private:

    static std::unique_ptr<ResourceManagerEditor> m_instance;
    static std::once_flag flag;

    RenderTexture rt_game_view {};
    ImFont* font {};

    ResourceManagerEditor(const ResourceManagerEditor&) = delete;
    ResourceManagerEditor& operator=(const ResourceManagerEditor&) = delete;

public:

    ResourceManagerEditor();
    ~ResourceManagerEditor();

    static ResourceManagerEditor* instance();

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