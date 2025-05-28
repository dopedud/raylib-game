#include "player_camera_controller.h"

#include "raylib.h"
#include "raymath.h"

PlayerCameraController::PlayerCameraController()
{
    m_camera.position = { .0f, 2.5f, -5.0f };
    m_camera.target = m_camera.position + (Vector3){ .0f, .0f, 1.0f };
    m_camera.up = { .0f, 1.0f, .0f };
    m_camera.fovy = 75.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void PlayerCameraController::move_right(float amount)
{
    m_camera.position += (Vector3){ -amount, .0f, .0f };
    m_camera.target = m_camera.position + (Vector3){ .0f, .0f, 1.0f };
}