/** @file */

#pragma once

#include <vector>
#include <utility>
#include <string>
#include <string_view>

#include "raylib.h"
#include "imgui.h"

#include "box2d/box2d.h"

/**
 * @namespace
 */
namespace resourcevars
{
    struct FRAMES_AMOUNT 
    {
        struct PLAYER
        {
            static constexpr int IDLE { 6 };
            static constexpr int RUN { 8 };
            static constexpr int SLIDE { 5 };
        };
    };

    struct TEXTUREPATH
    {
        struct PLAYER
        {
            static constexpr std::string_view IDLE { "../resources/warrior/idle/warrior_idle" };
            static constexpr std::string_view RUN { "../resources/warrior/run/warrior_run" };
            static constexpr std::string_view SLIDE { "../resources/warrior/slide/warrior_slide" };
        };
    };

    struct SHADERPATH
    {
        struct DEFAULT
        {
            static constexpr std::string_view SHADER { "../resources/shaders/glsl/default" };
        };

        struct PLAYER
        {
            static constexpr std::string_view SHADER { "../resources/shaders/glsl/player" };
        };
    };

    struct FONTPATH
    {
        static constexpr std::string_view CASCADIA_CODE { "../resources/CascadiaCode.ttf" };
    };
};

/**
 * @enum
 * @brief Enumeration for texture resources.
 */
enum class TextureResource
{
    PLAYER_IDLE,
    PLAYER_RUN,
    PLAYER_SLIDE,
    COUNT
};

/**
 * @enum
 * @brief Enumeration for shader resources.
 */
enum class ShaderResource
{
    DEFAULT,
    PLAYER,
    COUNT
};

/**
 * @enum
 * @brief Enumeration for model resources.
 * 
 * @note
 * Enumeration for model resources must have a 1 to 1 relationship with TextureResource. This is because the dimensions
 * of the model must exactly match the dimensions of the texture it is rendering.
 * Without this relationship, a single 3D model could host an amount of textures with different dimensions, each with
 * different wrappings onto the 3D model. 
 * It will just generally look weird and hence why this relationship should be maintained.
 */
enum class ModelResource
{
    PLAYER_IDLE,
    PLAYER_RUN,
    PLAYER_SLIDE,
    COUNT
};

/**
 * @class
 * @brief Game manager class to manage game resources.
 */
class ResourceManager
{
private:

    /**
     * @name List of Resources
     * @brief A list of resources to maintain and be fetched from throughout the lifetime of this class.
     * 
     * @{
     */

    b2WorldId m_world_id {}; /**< @public */

    std::vector<std::vector<Texture>> texture_resources {}; /**< @public */
    std::vector<Shader> shader_resources {};                /**< @public */
    std::vector<Model> model_resources {};                  /**< @public */

    RenderTexture rt_target {};

    ImFont* default_font {};

    /** @} */

    ResourceManager();
    ~ResourceManager();

    /**
     * @brief Delete copy constructors/assignment operators to disallow copying between resource managers.
     * @{
     */

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /** @} */

    /**
     * @param path Formatted path to textures. Paths are formatted (follow a certain regular expression) to allow
     *             animations.
     */
    std::vector<Texture> load_texture_resource(const int frame_count, const std::string_view path_sv);
    
    Shader load_shader_resource(const std::string_view path_sv);
    Shader load_shader_resource_fs(const std::string_view path_sv);

public:

    /**
     * @fn
     * @brief ResourceManager static instance.
     * 
     * ResourceManager follows a design pattern known as the Singleton pattern, in which only 1 instance of a class is
     * allowed at any given time. In C++, the implementation below is one of the common ways to implement such design
     * pattern.
     */
    inline static ResourceManager& instance()
    {
        // created only once, thread-safe in C++11+
        static ResourceManager instance {};
        return instance;
    }

    inline b2WorldId world_id() { return m_world_id; }

    inline std::vector<Texture>* get_texture_resource(const TextureResource texture_resource)
    {
        return &texture_resources[static_cast<int>(texture_resource)];
    }

    inline Shader* get_shader_resource(const ShaderResource shader_resource)
    {
        return &shader_resources[static_cast<int>(shader_resource)];
    }

    inline Model* get_model_resource(const ModelResource model_resource)
    {
        return &model_resources[static_cast<int>(model_resource)];
    }

    inline RenderTexture* get_rt()
    {
        return &rt_target;
    }

    inline ImFont* get_default_font_resource()
    {
        return default_font;
    }
};
