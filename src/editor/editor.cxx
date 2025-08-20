#include "editor.h"

#include "rcamera.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "rlImGui.h"

#include "resource_manager_editor.h"
#include "settings_editor.h"

std::unique_ptr<Editor> Editor::m_instance { nullptr };
std::mutex Editor::locker;

Editor::Editor(PrivateKey)
{
    settings_editor::initialise();
    ResourceManagerEditor::instance();
}

Editor::~Editor() 
{
    settings_editor::deinitialise();
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

        float max_speed { camera_controller.max_speed() };
        ImGui::SliderFloat("##camera_max_speed",
            &max_speed,
            CameraController::MAX_SPEED_MIN,
            CameraController::MAX_SPEED_MAX,
            "%.0f",
            ImGuiSliderFlags_AlwaysClamp
        );
        camera_controller.set_max_speed(max_speed);
        // TraceLog(LOG_DEBUG, "camera max speed: %f", camera_controller.max_speed());

        ImGui::TextWrapped("Camera Smooth Multiplier");

        float smooth_multiplier { camera_controller.smooth_multiplier() };
        ImGui::SliderFloat("##camera_smooth_multiplier",
            &smooth_multiplier,
            CameraController::SMOOTH_MULTIPLIER_MIN,
            CameraController::SMOOTH_MULTIPLIER_MAX,
            "%.0f",
            ImGuiSliderFlags_AlwaysClamp
        );
        camera_controller.set_smooth_multiplier(smooth_multiplier);
        TraceLog(LOG_DEBUG, "camera smooth multiplier: %f", camera_controller.smooth_multiplier());
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

Editor::CameraController::CameraController()
{
    m_camera.position = { .0f, .0f, .0f };
    m_camera.target = Vector3{ .0f, .0f, 1.0f };
    m_camera.up = { .0f, 1.0f, .0f };
    m_camera.fovy = 75.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void Editor::CameraController::handle_input()
{
    Vector2 mouse_delta { GetMouseDelta() * m_mouse_sensitivity * COMMON_FACTOR };

    if (Vector2Length(mouse_delta) > .0f)
    {
        float yaw { mouse_delta.x };
        float pitch { mouse_delta.y };
        
        CameraYaw(&m_camera, -yaw, false);
        CameraPitch(&m_camera, -pitch, true, false, false);
    }

    Vector3 local_move_vector {};
        
    if (IsKeyDown(KEY_W)) local_move_vector += Vector3UnitZ;
    if (IsKeyDown(KEY_S)) local_move_vector += Vector3Negate(Vector3UnitZ);
    if (IsKeyDown(KEY_D)) local_move_vector += Vector3UnitX;
    if (IsKeyDown(KEY_A)) local_move_vector += Vector3Negate(Vector3UnitX);
    if (IsKeyDown(KEY_SPACE)) local_move_vector += Vector3UnitY;
    if (IsKeyDown(KEY_LEFT_CONTROL)) local_move_vector += Vector3Negate(Vector3UnitY);
    
    local_move_vector = Vector3Normalize(local_move_vector);

    float max_speed { m_max_speed * COMMON_FACTOR };
    float smooth_multiplier { m_smooth_multiplier * COMMON_FACTOR };
    
    if (!FloatEquals(Vector3Length(local_move_vector), .0f))
    move_vector = Vector3Lerp(
		move_vector,
		Vector3Normalize(local_move_vector) * max_speed,
		smooth_multiplier
	);

    else move_vector = Vector3Lerp(
        move_vector,
        Vector3Zeros,
        smooth_multiplier
    );
    
    move_vector = Vector3ClampValue(move_vector, .0f, max_speed);
    
    CameraMoveForward(&m_camera, move_vector.z, false);
    CameraMoveRight(&m_camera, move_vector.x, false);
    
    // handle vertical movement in camera's local up direction
    if (!FloatEquals(move_vector.y, 0.0f))
    {
        // calculate camera's local coordinate system
        Vector3 forward { Vector3Normalize(Vector3Subtract(m_camera.target, m_camera.position)) };
        Vector3 right { Vector3Normalize(Vector3CrossProduct(forward, m_camera.up)) };
        Vector3 local_up { Vector3CrossProduct(right, forward) };
        
        // move both position and target to maintain camera orientation
        m_camera.position = Vector3Add(m_camera.position, Vector3Scale(local_up, move_vector.y));
        m_camera.target = Vector3Add(m_camera.target, Vector3Scale(local_up, move_vector.y));
    }
}