#include "animated_model.h"

#include <vector>
#include <string>
#include <numeric>
#include <cmath>

#include "raylib.h"

#include "settings.h"

AnimatedModel::AnimatedModel
(
    int frame_count,
    bool looping,
    char* textures_path,
    char* vertexshader_path,
    char* fragmentshader_path,
    std::vector<float> timing
) : 
frame_count { frame_count },
looping { looping },
timing { timing },
shader { LoadShader(vertexshader_path, fragmentshader_path) }
{
    textures.resize(frame_count);

    for (int i = 0; i < frame_count; i++)
    {
        std::string path = std::string(textures_path) + "_" + std::to_string(i) + ".png";
        textures[i] = LoadTexture(path.c_str());
    }

    m_model = LoadModelFromMesh(GenMeshCube(
        (float)textures[0].width / -TEXELS_PER_UNIT,
        (float)textures[0].height / -TEXELS_PER_UNIT,
        .0f
    ));

    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[0];
    m_model.materials[0].shader = shader;

    timing_cumulative.resize(timing.size());

    std::partial_sum(timing.cbegin(), timing.cend(), timing_cumulative.begin());

    duration = timing_cumulative[frame_count - 1];
}

AnimatedModel::AnimatedModel
(
    int frame_count,
    bool looping,
    char* textures_path,
    char* vertexshader_path,
    char* fragmentshader_path,
    float timing
) :
AnimatedModel 
{ 
    frame_count,
    looping,
    textures_path,
    vertexshader_path,
    fragmentshader_path,
    std::vector<float>(frame_count, timing)
}
{}

AnimatedModel::~AnimatedModel()
{
    for (int i = 0; i < frame_count; i++) UnloadTexture(textures[i]);

    UnloadModel(m_model);
    UnloadShader(shader);
}

void AnimatedModel::update()
{
    timer += GetFrameTime();

    if (looping) timer = fmodf(timer, duration);

    frameindex = AnimatedModel::bsearch_frameindex();

    m_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = textures[frameindex];
}

int AnimatedModel::bsearch_frameindex()
{
    if (timer >= .0f && timer < timing_cumulative[0]) return 0;
    else if (timer > duration) return frame_count - 1;

    int low {};
    int high { frame_count - 1 };

    while (low <= high)
    {
        int mid { (int)floorf((float)(low + high) / 2) };

        if (timer >= timing_cumulative[mid] && timer < timing_cumulative[mid + 1])
        return mid + 1;

        if (timer >= timing_cumulative[mid]) low = mid + 1;
        else high = mid - 1;
    }
}