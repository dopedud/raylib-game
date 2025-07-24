#pragma once

#include "raylib.h"

class CameraController
{
private:   

    Camera3D m_camera;
	
	static constexpr float MAX_SPEED { 1.0f };
	static constexpr float DRAG_MULTIPLIER { .25f };
	static constexpr float ACCELERATION_MULTIPLIER { .01f };
		
	Vector3 move_vector {};

public:

    CameraController();

    Camera3D camera() { return m_camera; }

    void follow(Transform target);

    void handle_move();
};