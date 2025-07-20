#pragma once

#include <vector>
#include <utility>

#include "raylib.h"

#include "resource_manager.h"
#include "settings.h"

/**
 * @class
 * @brief Game component class to animate textures on models.
 * 
 * AnimatedModel is effectively an animation on a 2D texture, but applied on a 3D model. This class is done because
 * Raylib did not provide such utility; it only provides 3D models with static textures and drawing 2D texture on a
 * texture object directly.
 * 
 * AnimatedModel assumes every texture assigned is of consistent dimension. In other words, every texture has the same
 * size. This is important to keep the model's dimension the same.
 * 
 * @note 
 * Else, if the model's dimension follows every texture's dimension, there would be an array of models to keep track
 * of, which is tedious. It is also important to note that doing it this way does have some trade-offs, notably for
 * textures that have alpha values (transparency). For textures that mostly contain alpha values, most of the surface
 * of the model aren't used, and so this might waste memory space (probably, I am not well-versed (yet) on how models
 * are actually rendered, and if the alpha values in the textures actually did anything to affect the performance of
 * game rendering).
 */
class AnimatedModel
{
private:

    bool looping {};

    Vector2 pivot {};

    /**
     * @brief Holds how long should each texture lasts in an animation.
     * 
     * A set of integer-float pairs that act as keyframes for the AnimatedModel where the integer part indicates the
     * order of a texture in the list of textures to animate, and the float part indicates how long should the texture
     * stay during the animation sequence.
     * 
     * For example, for a list of textures (for instance, @c texture1.png, @c texture2.png, and @c texture3.png) with
     * the given timings (0 - 1.0f, 1 - 2.0f, 2 - 1.0f), when played normally, would proceed as normal; play 1 second
     * for @c texture1.png, play 2 seconds for @c texture2.png, and play 1 second for @c texture3.png. One can modify
     * this and have a more complex animation playthrough, such as with the timings (0 - 1.0f, 1 - 2.0f, 0 - 2.0f, 2 -
     * 1.0f).This will play like before, with the 3rd frame being played at @c texture1.png again for 2 seconds.
     */
    std::vector<std::pair<int, float>> textures_timing {};

    int frameindex {};
    float timer {};
    float duration {};
    bool finished {};

    std::vector<Texture>* textures {};
    Shader* shader {};
    Model* m_model {};

    /**
     * @brief like textures_timing, but only keeping track of how many seconds has passed for each passing key frame.
     */
    std::vector<float> timing_cumulative {};

    /**
     * @brief Binary search to search for the playhead with respect to the frame index.
     * 
     * @return 
     */
    int bsearch_frameindex();

    void initialise_timings();

public:

    AnimatedModel() = default; /**< @private */

    /**
     * @fn
     * @brief The class's main constructor.
     * 
     * The class's main constructor does not need to take each value for each internal variable when initialising. It
     * will only need to take values that are set by the caller, and the rest are initialised automatically.
     * 
     * @param texture_resource  TextureResource enumeration that determines which set of textures this AnimatedModel
     *                          should have.
     * @param shader_resource   ShaderResource enumeration that determines which shader this AnimatedModel should have.
     * @param model_resource    ModelResource enumeration that determines which model this AnimatedModel should have.
     * @param looping           Determines whether this AnimatedModel loops.
     * @param pivot             The center of the texture relative to the surface of the model.
     * @param timing            @copydoc AnimatedModel::textures_timing()
     */
    AnimatedModel
    (
        TextureResource texture_resource,
        ShaderResource shader_resource,
        ModelResource model_resource,
        bool looping,
        Vector2 pivot,
        std::vector<std::pair<int, float>>&& timing
    );

    /**
     * @fn
     * @brief The class's delegated constructor.
     * 
     * The class's delegated constructor serves the same function as the main class constructor, but the timing is
     * duplicated into @c frame_count amount, each one lasts @c timing seconds.
     * 
     * Refer to the main class constructor for full list of function parameter explanation.
     * 
     * @param timing How long should each texture be played.
     */
    AnimatedModel
    (
        TextureResource texture_resource,
        ShaderResource shader_resource,
        ModelResource model_resource,
        bool looping,
        Vector2 pivot,
        float timing
    );
    

    AnimatedModel(const AnimatedModel& other);                  /**< @private */
    AnimatedModel(AnimatedModel&& other) noexcept;              /**< @private */
    AnimatedModel& operator=(const AnimatedModel& other);       /**< @private */
    AnimatedModel& operator=(AnimatedModel&& other) noexcept;   /**< @private */

    ~AnimatedModel(); /**< @private */

    float width() const { return static_cast<float>((*textures)[0].width) / settings::GENERAL::TEXELS_PER_UNIT; }
    float height() const { return static_cast<float>((*textures)[0].height) / settings::GENERAL::TEXELS_PER_UNIT; }

    Model* model() const { return m_model; }

    /**
     * @fn
     * @brief Function to animate texture on a model by a game's timestep.
     * @note It needs to be called every game update for the animation to play, which will likely be called inside a
     * @c while loop.
     */
    void animate();
};