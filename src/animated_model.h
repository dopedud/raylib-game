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

    Model m_model {};
    Shader shader {};

    // used binary search to search for the playhead in the perspective of the frame index
    int bsearch_frameindex();

public:
    AnimatedModel
    (
        int frame_count,
        bool looping,
        char* textures_path,
        char* vertexshader_path,
        char* fragmentshader_path,
        std::vector<float> timing
    );

    AnimatedModel
    (
        int frame_count,
        bool looping,
        char* textures_path,
        char* vertexshader_path,
        char* fragmentshader_path,
        float timing
    );

    ~AnimatedModel();

    const Model& model() const { return m_model; }

    void update();
};