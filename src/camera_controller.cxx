#include "camera_controller.h"

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

#include "settings.h"

CameraController::CameraController()
{
    m_camera.position = { 5.0f, 2.5f, -5.0f };
    m_camera.target = m_camera.position + (Vector3){ -1.0f, .0f, 1.0f };
    m_camera.up = { .0f, 1.0f, .0f };
    m_camera.fovy = 75.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::follow(Transform target)
{
    
}

void CameraController::handle_move()
{
    Vector3 local_move_vector {};
        
    if (IsKeyDown(KEY_W)) local_move_vector += Vector3UnitZ;
    if (IsKeyDown(KEY_S)) local_move_vector += Vector3Negate(Vector3UnitZ);
    if (IsKeyDown(KEY_D)) local_move_vector += Vector3UnitX;
    if (IsKeyDown(KEY_A)) local_move_vector += Vector3Negate(Vector3UnitX);
    if (IsKeyDown(KEY_SPACE)) local_move_vector += Vector3UnitY;
    if (IsKeyDown(KEY_LEFT_CONTROL)) local_move_vector += Vector3Negate(Vector3UnitY);
    
    local_move_vector = Vector3Normalize(local_move_vector);
    
    if (!FloatEquals(Vector3Length(local_move_vector), .0f))
    move_vector = Vector3Lerp(
		move_vector,
		Vector3Normalize(local_move_vector) * MAX_SPEED,
		SMOOTH_MULTIPLIER
	);

    else move_vector = Vector3Lerp(
        move_vector,
        Vector3Zeros,
        SMOOTH_MULTIPLIER
    );
    
    move_vector = Vector3ClampValue(move_vector, .0f, MAX_SPEED);
    
    CameraMoveForward(&m_camera, move_vector.z, false);
    CameraMoveUp(&m_camera, move_vector.y);
    CameraMoveRight(&m_camera, move_vector.x, false);
}