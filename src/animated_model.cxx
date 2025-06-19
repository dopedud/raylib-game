#include "animated_model.h"

#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <utility>

#include "raylib.h"

#include "settings.h"
#include "resource_manager.h"

AnimatedModel::AnimatedModel
(
    int frame_count,
    bool looping,
    Vector2 pivot,
    std::vector<std::pair<int, float>> textures_timing
)
: frame_count           { frame_count }
, looping               { looping }
, pivot                 { pivot }
, shader                { ResourceManager::instance().player_shader() }
, textures_timing       { textures_timing }
{
    textures.resize(frame_count);

    for (int i = 0; i < frame_count; i++)
    {
        auto& player_idle_textures = ResourceManager::instance().player_idle();
        textures[i] = player_idle_textures[i];
    }

    m_width = (float)textures[0].width / TEXELS_PER_UNIT;
    m_height = (float)textures[0].height / TEXELS_PER_UNIT ;

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
    float timing
) 
: AnimatedModel 
{ 
    frame_count,
    looping,
    pivot,
    {}
}
{
    for (int i = 0; i < frame_count; i++)
    textures_timing.emplace_back(std::pair<int, float>{ i, timing });

    AnimatedModel::initialise_timings();
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
    if (finished) return;

    timer += GetFrameTime();

    if (looping) timer = fmodf(timer, duration);
    else if (timer >= duration)
    {
        timer = .0f;
        finished = true;
    }

    frameindex = AnimatedModel::bsearch_frameindex();

    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[frameindex];
}

int AnimatedModel::bsearch_frameindex()
{
    int total_frame_count { textures_timing.size() };

    if (timer >= .0f && timer < timing_cumulative[0]) return 0;
    else if (timer >= duration) return total_frame_count - 1;

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
        timing_cumulative.emplace_back(acc);
    }

    duration = timing_cumulative[frame_count - 1];
}