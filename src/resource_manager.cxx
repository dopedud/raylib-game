#include "resource_manager.h"

#include <string>

#include "raylib.h"
#include "box2d/box2d.h"

#include "settings.h"

std::unique_ptr<ResourceManager> ResourceManager::m_instance { nullptr };
std::once_flag ResourceManager::flag;

ResourceManager* ResourceManager::instance()
{
    std::call_once(flag, []()
    {
        m_instance = std::make_unique<ResourceManager>();
    });

    return m_instance.get();
}

ResourceManager::ResourceManager()
{
    b2WorldDef world_def { b2DefaultWorldDef() };

    b2SetLengthUnitsPerMeter(1.0f);
    world_def.gravity.y = settings::PHYSICS::GRAVITY_ACCELERATION;

    m_world_id = b2CreateWorld(&world_def);

    using namespace resourcevars;

    for (int i = 0; i < static_cast<int>(TextureResource::COUNT); i++)
    {
        texture_resources.emplace_back(std::vector<Texture>{});

        switch (i)
        {
            case static_cast<int>(TextureResource::PLAYER_IDLE):
                texture_resources[i] =
                load_texture_resource(FRAMES_AMOUNT::PLAYER::IDLE, TEXTUREPATH::PLAYER::IDLE);
            break;

            case static_cast<int>(TextureResource::PLAYER_RUN):
                texture_resources[i] = 
                load_texture_resource(FRAMES_AMOUNT::PLAYER::RUN, TEXTUREPATH::PLAYER::RUN);
            break;

            case static_cast<int>(TextureResource::PLAYER_SLIDE):
                texture_resources[i] =
                load_texture_resource(FRAMES_AMOUNT::PLAYER::SLIDE, TEXTUREPATH::PLAYER::SLIDE);
            break;
        }
    }

    for (int i = 0; i < static_cast<int>(ShaderResource::COUNT); i++)
    {
        switch (i)
        {
            case static_cast<int>(ShaderResource::PLAYER):
                shader_resources.emplace_back(load_shader_resource(SHADERPATH::PLAYER));
            break;

            case static_cast<int>(ShaderResource::DEFAULT):
                shader_resources.emplace_back(load_shader_resource(SHADERPATH::DEFAULT));
            break;
        }
    }

    for (int i = 0; i < static_cast<int>(ModelResource::COUNT); i++)
    {
        std::vector<Texture> textures {};

        switch (i)
        {
            case static_cast<int>(ModelResource::PLAYER_IDLE):
                textures = *texture_resource(TextureResource::PLAYER_IDLE);
            break;

            case static_cast<int>(ModelResource::PLAYER_RUN):
                textures = *texture_resource(TextureResource::PLAYER_RUN);
            break;

            case static_cast<int>(ModelResource::PLAYER_SLIDE):
                textures = *texture_resource(TextureResource::PLAYER_SLIDE);
            break;
        }

        float width {};
        float height {};

        if (textures.size() > 0)
        {
            width = (static_cast<float>(textures[0].width)) / settings::GENERAL::TEXELS_PER_UNIT;
            height = (static_cast<float>(textures[0].height)) / settings::GENERAL::TEXELS_PER_UNIT;
        } else TraceLog(LOG_WARNING, "Error reading texture resources array, setting width and height to 0 to create model...");

        model_resources.emplace_back(LoadModelFromMesh(
            GenMeshCube(width, -height, .0f)
        ));
    }
}

ResourceManager::~ResourceManager()
{
    b2DestroyWorld(m_world_id);

    for (auto& texture_resource : texture_resources)
    for (auto& texture : texture_resource)
    UnloadTexture(texture);

    for (auto& shader : shader_resources)
    UnloadShader(shader);

    for (auto& model : model_resources)
    UnloadModel(model);
}

std::vector<Texture> ResourceManager::load_texture_resource(const int count, const std::string_view path_sv)
{
    std::string path { std::string(path_sv) };

    std::vector<Texture> textures {};

    textures.resize(count);

    for (int i = 0; i < count; i++)
    {
        std::string formatted_path { path + "_" + std::to_string(i) + ".png" };

        if (!FileExists(formatted_path.c_str()))
        {
            TraceLog(LOG_WARNING, "Error loading texture resource at path: %s, returning empty set of textures...", formatted_path.c_str());
            return {};
        }

        textures[i] = LoadTexture(formatted_path.c_str());
    }

    return textures;
}

Shader ResourceManager::load_shader_resource(const std::string_view path_sv)
{
    std::string path { std::string(path_sv) };
    return LoadShader((path + ".vs").c_str(), (path + ".fs").c_str());
}

Shader ResourceManager::load_shader_resource_fs(const std::string_view path_sv)
{
    std::string path { std::string(path_sv) };
    return LoadShader(0, (path + ".fs").c_str());
}