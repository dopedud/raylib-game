#pragma once

#include <vector>

#include "raylib.h"

class AnimatedModel
{
    int frame_count {};
    int frameindex {};

    float timer {};
    float duration {};

    bool looping {};

    std::vector<float> timing;
    std::vector<float> timing_cumulative;

    // NOTE: path to textures are formatted to allow animations
    std::vector<Texture> textures;
    Vector2 pivot {};

    Model m_model {};
    Shader shader {};

    float m_width {};
    float m_height {};

    // used binary search to search for the playhead in the perspective of the frame index
    int bsearch_frameindex();

public:
    AnimatedModel
    (
        int frame_count,
        bool looping,
        char* textures_path,
        Vector2 pivot,
        char* vertexshader_path,
        char* fragmentshader_path,
        std::vector<float> timing
    );

    AnimatedModel
    (
        int frame_count,
        bool looping,
        char* textures_path,
        Vector2 pivot,
        char* vertexshader_path,
        char* fragmentshader_path,
        float timing
    );

    ~AnimatedModel();

    constexpr float width() { return m_width; }
    constexpr float height() { return m_height; }

    const Model& model() const { return m_model; }

    void animate();
};