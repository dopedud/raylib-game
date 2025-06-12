#include "animated_model.h"

#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <utility>

#include "raylib.h"

#include "settings.h"

AnimatedModel::AnimatedModel
(
    int frame_count,
    bool looping,
    Vector2 pivot,
    std::string_view textures_path,
    std::string_view vertexshader_path,
    std::string_view fragmentshader_path,
    std::vector<std::pair<int, float>> textures_timing
) : 
frame_count { frame_count },
looping { looping },
pivot { pivot },
textures_timing { textures_timing },
shader { LoadShader(vertexshader_path.data(), fragmentshader_path.data()) },
textures_path { textures_path },
vertexshader_path { vertexshader_path },
fragmentshader_path { fragmentshader_path }
{
    textures.resize(frame_count);

    for (int i = 0; i < frame_count; i++)
    {
        std::string path = std::string{ textures_path } + "_" + std::to_string(i) + ".png";
        textures[i] = LoadTexture(path.c_str());
    }

    m_width = (float)textures[0].width / TEXELS_PER_UNIT;
    m_height = (float)textures[0].height / TEXELS_PER_UNIT;

    m_model = LoadModelFromMesh(GenMeshCube(-m_width, -m_height, .0f));

    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[0];
    m_model.materials[0].shader = shader;

    AnimatedModel::initialise_timings();
}

AnimatedModel::AnimatedModel
(
    int frame_count,
    bool looping,
    Vector2 pivot,
    std::string_view textures_path,
    std::string_view vertexshader_path,
    std::string_view fragmentshader_path,
    float timing
) :
AnimatedModel 
{ 
    frame_count,
    looping,
    pivot,
    textures_path,
    vertexshader_path,
    fragmentshader_path,
    {{}}
}
{
    for (int i = 0; i < frame_count; i++)
    textures_timing.push_back({ i, timing });

    AnimatedModel::initialise_timings();
}

AnimatedModel::AnimatedModel(AnimatedModel& other) :
frame_count { other.frame_count },
duration { other.duration },
looping { other.looping },
pivot { other.pivot },
textures_timing { other.textures_timing },
timing_cumulative { other.timing_cumulative },
m_width { other.m_width },
m_height { other.m_height },
textures_path { other.textures_path },
vertexshader_path { other.vertexshader_path },
fragmentshader_path { other.fragmentshader_path }
{
    textures.resize(other.textures.size());

    for (size_t i = 0; i < other.textures.size(); ++i) 
    {
        std::string path = std::string{ textures_path } + "_" + std::to_string(i) + ".png";
        textures[i] = LoadTexture(path.c_str());
    }

    shader = LoadShader(vertexshader_path.data(), fragmentshader_path.data());

    m_model = LoadModelFromMesh(GenMeshCube(-m_width, -m_height, 0.0f));
    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[frameindex];
    m_model.materials[0].shader = shader;
}


AnimatedModel::AnimatedModel(AnimatedModel&& other) noexcept :
frame_count { other.frame_count },
duration { other.duration },
looping { other.looping },
pivot { other.pivot },
textures_timing { std::move(other.textures_timing) },
timing_cumulative { std::move(other.timing_cumulative) },
textures { std::move(other.textures) },
m_model { other.m_model },
shader { other.shader },
m_width { other.m_width },
m_height { other.m_height },
textures_path { other.textures_path },
vertexshader_path { other.vertexshader_path },
fragmentshader_path { other.fragmentshader_path }
{
    other.frame_count = 0;
    other.frameindex = 0;
    other.timer = .0f;
    other.duration = .0f;
    other.looping = false;
    other.finished = false;
    other.pivot = { 0, 0 };
    other.m_width = .0f;
    other.m_height = .0f;

    other.m_model = {};
    other.shader = {};
    other.textures.clear();
}

AnimatedModel& AnimatedModel::operator=(AnimatedModel&& other) noexcept
{
    if (this == &other) return *this;

    for (Texture texture : textures) UnloadTexture(texture);

    UnloadModel(m_model);
    UnloadShader(shader);

    frame_count = other.frame_count;
    frameindex = other.frameindex;
    timer = other.timer;
    duration = other.duration;
    looping = other.looping;
    pivot = other.pivot;
    textures = std::move(other.textures);
    textures_timing = std::move(other.textures_timing);
    timing_cumulative = std::move(other.timing_cumulative);

    m_width = other.m_width;
    m_height = other.m_height;
    m_model = other.m_model;
    shader = other.shader;

    other.frame_count = 0;
    other.frameindex = 0;
    other.timer = .0f;
    other.duration = .0f;
    other.looping = false;
    other.finished = false;
    other.pivot = { 0, 0 };
    other.m_width = .0f;
    other.m_height = .0f;

    other.m_model = {};
    other.shader = {};
    other.textures.clear();

    return *this;
}

AnimatedModel::~AnimatedModel()
{
    TraceLog(LOG_DEBUG, "was this called?");

    for (Texture texture : textures) UnloadTexture(texture);

    UnloadModel(m_model);

    // only unload if it's a valid shader
    if (shader.id > 0) UnloadShader(shader);
}

void AnimatedModel::animate()
{
    timer += GetFrameTime();

    if (looping) timer = fmodf(timer, duration);

    frameindex = AnimatedModel::bsearch_frameindex();

    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[frameindex];
}

int AnimatedModel::bsearch_frameindex()
{
    int total_frame_count { textures_timing.size() };

    if (timer >= .0f && timer < timing_cumulative[0]) return 0;
    else if (timer > duration) return total_frame_count - 1;

    int low {};
    int high { total_frame_count - 1 };

    while (low <= high)
    {
        int mid { (int)floorf((float)(low + high) / 2) };

        if (timer >= timing_cumulative[mid] && timer < timing_cumulative[mid + 1])
        return mid + 1;

        if (timer >= timing_cumulative[mid]) low = mid + 1;
        else high = mid - 1;
    }
}

void AnimatedModel::initialise_timings()
{
    timing_cumulative.resize(textures_timing.size());
    
    float acc {};
    for (const std::pair<int, float>& pair : textures_timing)
    {
        acc += pair.second;
        timing_cumulative.push_back(acc);
    }

    duration = timing_cumulative[frame_count - 1];
}