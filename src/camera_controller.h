#pragma once

#include "raylib.h"

class CameraController
{
private:   

    Camera3D m_camera;
	
	static constexpr float MAX_SPEED { .01f };
	static constexpr float SMOOTH_MULTIPLIER { .1f };
		
	Vector3 move_vector {};

public:

    CameraController();

    Camera3D camera() { return m_camera; }

    void follow(Transform target);

    void handle_move();
};