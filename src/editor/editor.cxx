#include "editor.h"

#include <memory>

#include "imgui.h"
#include "imgui_internal.h"
#include "rlImGui.h"

#include "resource_manager_editor.h"
#include "settings_editor.h"
#include "camera_controller.h"

std::unique_ptr<Editor> Editor::m_instance { nullptr };
std::mutex Editor::locker;

Editor::Editor(PrivateKey)
{
    settings_editor::initialise();
    ResourceManagerEditor::instance();
}

Editor::~Editor() 
{
    settings::deinitialise();
    ResourceManagerEditor::destroy();
}

Editor* Editor::instance()
{
    std::lock_guard<std::mutex> lock { locker };
    if (!m_instance) m_instance = std::make_unique<Editor>(PrivateKey{});
    return m_instance.get();
}

void Editor::destroy()
{
    std::lock_guard<std::mutex> lock { locker };
    m_instance.reset();
}

void Editor::draw()
{
    rlImGuiBegin();
    ImGui::PushFont(ResourceManagerEditor::instance()->font_resource(), 20.0f);

    ImGuiViewport* viewport { ImGui::GetMainViewport() };
    ImGui::SetNextWindowViewport(viewport->ID);
    ImVec2 WorkPos { viewport->WorkPos };
    ImVec2 WorkSize { viewport->WorkSize };

    ImGui::SetNextWindowPos(WorkPos);
    ImGui::SetNextWindowSize(WorkSize);
    ImGui::Begin("Main", nullptr, 
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |

        ImGuiWindowFlags_MenuBar
    );
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(), ImGuiDockNodeFlags_None);

        static bool show_demo {};
        static bool show_metrics {};

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show Demo Window", nullptr, &show_demo);
                ImGui::MenuItem("Show Metrics Window", nullptr, &show_metrics);
                ImGui::EndMenu();
            }

            ImGui::MenuItem("About");

            ImGui::EndMenuBar();
        }
    ImGui::End();

    if (show_demo) ImGui::ShowDemoWindow();
    if (show_metrics) ImGui::ShowMetricsWindow();

    // SETUP DOCK WINDOWS ONCE
    static bool init_dock { true };
    if (init_dock)
    {
        init_dock = false;

        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, WorkSize);

        ImGuiID left {};
        ImGuiID right {};

        ImGui::DockBuilderSplitNode(
            dockspace_id,
            ImGuiDir_Left,
            .3f,
            &left,
            &right
        );

        ImGui::DockBuilderDockWindow("Variables", left);
        ImGui::DockBuilderDockWindow("Viewport", right);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::Begin("Variables", nullptr, ImGuiWindowFlags_NoCollapse);

        ImGui::TextWrapped("Camera Max Speed");

        float max_speed { camera.max_speed() };
        ImGui::SliderFloat("##camera_max_speed",
            &max_speed,
            CameraController::MAX_SPEED_MIN,
            CameraController::MAX_SPEED_MAX,
            "%.0f",
            ImGuiSliderFlags_AlwaysClamp
        );
        camera.set_max_speed(max_speed);

        ImGui::TextWrapped("Camera Smooth Multiplier");

        float smooth_multiplier { camera.smooth_multiplier() };
        ImGui::SliderFloat("##camera_smooth_multiplier",
            &smooth_multiplier,
            CameraController::SMOOTH_MULTIPLIER_MIN,
            CameraController::SMOOTH_MULTIPLIER_MAX,
            "%.0f",
            ImGuiSliderFlags_AlwaysClamp
        );
        camera.set_smooth_multiplier(smooth_multiplier);
    ImGui::End();
        
    ImGui::Begin("Viewport", nullptr, 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_HorizontalScrollbar
    );
        if (ImGui::BeginMenuBar())
        {
            ImGui::MenuItem("Setings");
            ImGui::EndMenuBar();
        }

        ImVec2 content_region { ImGui::GetContentRegionAvail() };

        RenderTexture* game_view { ResourceManagerEditor::instance()->game_view() };

        if ((float)game_view->texture.width != content_region.x ||
        (float)game_view->texture.height != content_region.y)
        game_view = ResourceManagerEditor::instance()->reload_game_view(content_region);

        rlImGuiImageRenderTexture(game_view);
    ImGui::End();

    ImGui::PopFont();
    rlImGuiEnd();
}