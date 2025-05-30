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
    char* textures_path,
    Vector2 pivot,
    char* vertexshader_path,
    char* fragmentshader_path,
    std::vector<std::pair<int, float>> textures_timing
) : 
frame_count { frame_count },
looping { looping },
pivot { pivot },
textures_timing { textures_timing },
shader { LoadShader(vertexshader_path, fragmentshader_path) }
{
    textures.resize(frame_count);

    for (int i = 0; i < frame_count; i++)
    {
        std::string path = std::string(textures_path) + "_" + std::to_string(i) + ".png";
        textures[i] = LoadTexture(path.c_str());
    }

    m_width = (float)textures[0].width / TEXELS_PER_UNIT;
    m_height = (float)textures[0].height / TEXELS_PER_UNIT;

    m_model = LoadModelFromMesh(GenMeshCube(-m_width, -m_height, .0f));

    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[0];
    m_model.materials[0].shader = shader;

    initialise_timings();
}

AnimatedModel::AnimatedModel
(
    int frame_count,
    bool looping,
    char* textures_path,
    Vector2 pivot,
    char* vertexshader_path,
    char* fragmentshader_path,
    float timing
) :
AnimatedModel 
{ 
    frame_count,
    looping,
    textures_path,
    pivot,
    vertexshader_path,
    fragmentshader_path,
    {{}}
}
{
    textures_timing.resize(frame_count);

    for (int i = 0; i < frame_count; i++)
    {
        textures_timing.push_back({ i, timing });
    }

    initialise_timings();
}

AnimatedModel::~AnimatedModel()
{
    for (int i = 0; i < frame_count; i++) UnloadTexture(textures[i]);

    UnloadModel(m_model);
    UnloadShader(shader);
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