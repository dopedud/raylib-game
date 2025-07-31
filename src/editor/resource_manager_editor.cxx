#include "resource_manager_editor.h"

#include "imgui.h"

#include "resource_manager.h"

ResourceManagerEditor::ResourceManagerEditor()
{
    using namespace resourcevars;

    rt_game_view = LoadRenderTexture(1024, 1024);

    ImGuiIO& io { ImGui::GetIO() };
    io.IniFilename = NULL;
    font = io.Fonts->AddFontFromFileTTF(FONTPATH::CASCADIA_CODE.data());
    if (font == nullptr) io.Fonts->AddFontDefault();
}

ResourceManagerEditor::~ResourceManagerEditor()
{
    UnloadRenderTexture(rt_game_view);
}

RenderTexture* ResourceManagerEditor::reload_game_view(ImVec2 viewport)
{
    UnloadRenderTexture(rt_game_view);

    rt_game_view = LoadRenderTexture(viewport.x, viewport.y);

    return &rt_game_view;
}