#pragma once

#include <memory>
#include <mutex>

#include "raylib.h"
#include "raymath.h"

#include "settings.h"

class Editor
{
private:

    struct PrivateKey { explicit PrivateKey() = default; };

    static std::unique_ptr<Editor> m_instance;
    static std::mutex locker;

    class CameraController
    {
    private:   

        Camera m_camera {};
        
        float m_max_speed { 50.0f };
        float m_smooth_multiplier { 25.0f };
        float m_mouse_sensitivity { 1.0f };

        static constexpr float COMMON_FACTOR { settings::INPUT::TIMESTEP / 2.0f };

        Vector3 move_vector {};

    public:

        static constexpr float MAX_SPEED_MIN { 1.0f };
        static constexpr float MAX_SPEED_MAX { 100.0f };

        static constexpr float SMOOTH_MULTIPLIER_MIN { 1.0f };
        static constexpr float SMOOTH_MULTIPLIER_MAX { 100.0f };

        static constexpr float MOUSE_SENSITIVITY_MIN { 1.0f };
        static constexpr float MOUSE_SENSITIVITY_MAX { 100.0f };

        CameraController();

        inline Camera3D camera() const { return m_camera; }

        inline float max_speed() const { return m_max_speed; }
        inline void set_max_speed(float value)
        { 
            m_max_speed = Clamp(value, MAX_SPEED_MIN, MAX_SPEED_MAX);
        }

        inline float smooth_multiplier() const { return m_smooth_multiplier; }
        inline void set_smooth_multiplier(float value)
        { 
            m_smooth_multiplier = Clamp(value, SMOOTH_MULTIPLIER_MIN, SMOOTH_MULTIPLIER_MAX);
        }

        inline float mouse_sensitivity() const { return m_mouse_sensitivity; }
        inline void set_mouse_sensitivity(float value)
        { 
            m_mouse_sensitivity = Clamp(value, MOUSE_SENSITIVITY_MIN, MOUSE_SENSITIVITY_MAX);
        }

        void handle_input();
    };

    CameraController camera_controller {};

public:

    explicit Editor(PrivateKey);
    ~Editor();

    static Editor* instance();
    static void destroy();

    void draw();
};