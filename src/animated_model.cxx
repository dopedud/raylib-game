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
    TextureResource texture_resource,
    ShaderResource shader_resource,
    ModelResource model_resource,
    bool looping,
    Vector2 pivot,
    std::vector<std::pair<int, float>>& timing
)
: textures          { ResourceManager::instance().get_texture_resource(texture_resource) }
, shader            { ResourceManager::instance().get_shader_resource(shader_resource) }
, m_model           { ResourceManager::instance().get_model_resource(model_resource) }
, looping           { looping }
, pivot             { pivot }
, textures_timing   { timing }
{
    m_model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = (*textures)[0];
    m_model->materials[0].shader = *shader;

    AnimatedModel::initialise_timings();
}

AnimatedModel::AnimatedModel
(
    TextureResource texture_resource,
    ShaderResource shader_resource,
    ModelResource model_resource,
    bool looping,
    Vector2 pivot,
    float timing
) 
: AnimatedModel 
{ 
    texture_resource,
    shader_resource,
    model_resource,
    looping,
    pivot,
    {}
}
{
    int frame_count {};

    switch (texture_resource)
    {
        case TextureResource::PLAYER_IDLE:
            frame_count = resourcevars::FRAMES_AMOUNT_PLAYER_IDLE;
        break;

        case TextureResource::PLAYER_RUN:
            frame_count = resourcevars::FRAMES_AMOUNT_PLAYER_RUN;
        break;

        case TextureResource::PLAYER_SLIDE:
            frame_count = resourcevars::FRAMES_AMOUNT_PLAYER_SLIDE;
        break;
    }
    
    for (int i = 0; i < frame_count; i++)
    textures_timing.emplace_back(std::pair<int, float>{ i, timing });

    AnimatedModel::initialise_timings();
}

AnimatedModel::AnimatedModel(const AnimatedModel& other)
: looping           { other.looping }
, pivot             { other.pivot }
, textures_timing   { other.textures_timing }
, frameindex        { other.frameindex }
, timer             { other.timer }
, duration          { other.duration }
, finished          { other.finished }
, textures          { other.textures }
, shader            { other.shader }
, m_model           { other.m_model }
, timing_cumulative { other.timing_cumulative }
{
}

AnimatedModel::AnimatedModel(AnimatedModel&& other) noexcept
: looping           { other.looping }
, pivot             { other.pivot }
, textures_timing   { std::move(other.textures_timing) }
, frameindex        { other.frameindex }
, timer             { other.timer }
, duration          { other.duration }
, finished          { other.finished }
, textures          { other.textures }
, shader            { other.shader }
, m_model           { other.m_model }
, timing_cumulative { std::move(other.timing_cumulative) }
{
    other.textures = nullptr;
    other.shader = nullptr;
    other.m_model = nullptr;
    
    other.frameindex = 0;
    other.timer = .0f;
    other.duration = .0f;
    other.finished = false;
}

AnimatedModel& AnimatedModel::operator=(const AnimatedModel& other)
{
    if (this == &other) return *this;
    
    looping             = other.looping;
    pivot               = other.pivot;
    textures_timing     = other.textures_timing;
    frameindex          = other.frameindex;
    timer               = other.timer;
    duration            = other.duration;
    finished            = other.finished;
    timing_cumulative   = other.timing_cumulative;
    
    // copy the resource pointers (shallow copy)
    textures = other.textures;
    shader = other.shader;
    m_model = other.m_model;
    
    return *this;
}

AnimatedModel& AnimatedModel::operator=(AnimatedModel&& other) noexcept
{
    if (this == &other) return *this;

    looping             = other.looping;
    pivot               = other.pivot;
    textures_timing     = std::move(other.textures_timing);
    frameindex          = other.frameindex;
    timer               = other.timer;
    duration            = other.duration;
    finished            = other.finished;
    timing_cumulative   = std::move(other.timing_cumulative);
    
    // transfer ownership of pointers
    textures = other.textures;
    shader = other.shader;
    m_model = other.m_model;

    other.textures = nullptr;
    other.shader = nullptr;
    other.m_model = nullptr;

    other.frameindex = 0;
    other.timer = .0f;
    other.duration = .0f;
    other.finished = false;
    
    return *this;
}

AnimatedModel::~AnimatedModel()
{
    textures = nullptr;
    shader = nullptr;
    m_model = nullptr;
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

    m_model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = (*textures)[frameindex];
}

int AnimatedModel::bsearch_frameindex()
{
    int total_frame_count { textures_timing.size() };

    if (timer >= .0f && timer < timing_cumulative[0]) return textures_timing[0].first;
    else if (timer >= duration) return textures_timing[total_frame_count - 1].first;

    int low {};
    int high { total_frame_count - 1 };

    while (low <= high)
    {
        int mid { (int)floorf((float)(low + high) / 2) };

        if (timer >= timing_cumulative[mid] && timer < timing_cumulative[mid + 1])
        return textures_timing[mid + 1].first;

        if (timer >= timing_cumulative[mid]) low = mid + 1;
        else high = mid - 1;
    }
    
    return 0;
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

    duration = timing_cumulative[textures_timing.size() - 1];
}