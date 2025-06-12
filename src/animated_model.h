#pragma once

#include <vector>
#include <utility>
#include <string_view>

#include "raylib.h"

/*
** NOTE: AnimatedModel assumes every texture assigned is of consistent dimension; in other words, every texture has the
** same size; this is important to keep the model's dimension the same 
**
** SIDE NOTE: else if the model's dimension follows every texture's dimension, there would be an array of models to
** keep track of, which is tedious; it is also important to note that doing it this way does have some trade-off,
** notably for textures that have alpha values (transparency); for textures that have many alpha values, most of the
** surface of the model aren't used, and so this might waste memory space (probably, I am not well-versed (yet) on how
** models are actually rendered, and if the alpha values in the textures actually did anything to affect the
** performance of game rendering)
*/
class AnimatedModel
{
private:
    int frame_count {};
    int frameindex {};

    float timer {};
    float duration {};

    bool looping {};
    bool finished {};

    std::vector<std::pair<int, float>> textures_timing;
    std::vector<float> timing_cumulative;

    // NOTE: path to textures are formatted to allow animations
    std::vector<Texture> textures;
    Vector2 pivot {};

    Model m_model {};
    Shader shader {};

    float m_width {};
    float m_height {};

    // to be used by the copy/move constructors/assignment operators
    std::string_view textures_path {};
    std::string_view vertexshader_path {};
    std::string_view fragmentshader_path {};

    // used binary search to search for the playhead in the perspective of the frame index
    int bsearch_frameindex();

    void initialise_timings();

public:
    AnimatedModel
    (
        int frame_count,
        bool looping,
        Vector2 pivot,
        std::string_view textures_path,
        std::string_view vertexshader_path,
        std::string_view fragmentshader_path,
        std::vector<std::pair<int, float>> textures_timing
    );

    AnimatedModel
    (
        int frame_count,
        bool looping,
        Vector2 pivot,
        std::string_view textures_path,
        std::string_view vertexshader_path,
        std::string_view fragmentshader_path,
        float timing
    );

    AnimatedModel(AnimatedModel& other);
    AnimatedModel(AnimatedModel&& other) noexcept;
    AnimatedModel& operator=(AnimatedModel& other);
    AnimatedModel& operator=(AnimatedModel&& other) noexcept;

    ~AnimatedModel();

    const float width() const { return m_width; }
    const float height() const { return m_height; }

    const Model& model() const { return m_model; }

    void animate();
};