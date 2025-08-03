#pragma once

#include <string_view>

/**
 * @namespace
 * @brief Resource variables to store paths to resources.
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
        static constexpr std::string_view DEFAULT { "../resources/shaders/glsl/default" };
        static constexpr std::string_view PLAYER { "../resources/shaders/glsl/player" };
    };

    struct FONTPATH
    {
        static constexpr std::string_view CASCADIA_CODE { "../resources/CascadiaCode.ttf" };
        static constexpr std::string_view GOOGLE_SANS_CODE { "../resources/GoogleSansCode.ttf" };
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