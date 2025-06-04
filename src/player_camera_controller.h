#pragma once

#include "raylib.h"

class PlayerCameraController
{
private:   
    Camera3D m_camera;

public:
    PlayerCameraController();

    Camera3D camera() { return m_camera; }

    void follow(Transform target);

    void move_right(float amount);
};