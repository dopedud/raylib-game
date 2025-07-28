#include "camera_controller.h"

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

CameraController::CameraController()
{
    m_camera.position = { 5.0f, 2.5f, -5.0f };
    m_camera.target = m_camera.position + (Vector3){ .0f, .0f, 1.0f };
    m_camera.up = { .0f, 1.0f, .0f };
    m_camera.fovy = 75.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::follow(Transform target)
{
    
}

void CameraController::handle_input()
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