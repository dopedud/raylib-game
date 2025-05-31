#pragma once

#include <vector>
#include <utility>
#include <string_view>

#include "raylib.h"

class AnimatedModel
{
    int frame_count {};
    int frameindex {};

    float timer {};
    float duration {};

    bool looping {};
    bool finished {};

    std::vector<float> timing;
    std::vector<std::pair<int, float>> textures_timing;
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

    void initialise_timings();

public:
    AnimatedModel
    (
        int frame_count,
        bool looping,
        std::string_view textures_path,
        Vector2 pivot,
        std::string_view vertexshader_path,
        std::string_view fragmentshader_path,
        std::vector<std::pair<int, float>> textures_timing
    );

    AnimatedModel
    (
        int frame_count,
        bool looping,
        std::string_view textures_path,
        Vector2 pivot,
        std::string_view vertexshader_path,
        std::string_view fragmentshader_path,
        float timing
    );

    ~AnimatedModel();

    const float width() const { return m_width; }
    const float height() const { return m_height; }

    const Model& model() const { return m_model; }

    void animate();
};