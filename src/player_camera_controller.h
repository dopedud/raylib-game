#pragma once

#include "raylib.h"

class PlayerCameraController
{
    
    Camera3D m_camera;

public:
    PlayerCameraController();

    Camera3D camera() { return m_camera; }

    void move_right(float amount);
};