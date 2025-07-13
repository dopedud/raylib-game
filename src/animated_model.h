#pragma once

#include <vector>
#include <utility>

#include "raylib.h"

#include "resource_manager.h"

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
     */
    std::vector<std::pair<int, float>> textures_timing {};

    int frameindex {};
    float timer {};
    float duration {};
    bool finished {};

    std::vector<Texture>* textures {};
    Shader* shader {};
    Model* m_model {};

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
     * @param timing            The set of integer float pairs that determines the sequence of textures by their
     *                          orders, and for how long will it be played. For example, for the set [(1, 1.0f), (2,
     *                          1.0f), (1, 2.0f)], the 1st texture will be played for 1 second, followed by the 2nd
     *                          texture for 1 second, and lastly the 1st texture again for 2 seconds.
     */
    AnimatedModel
    (
        TextureResource texture_resource,
        ShaderResource shader_resource,
        ModelResource model_resource,
        bool looping,
        Vector2 pivot,
        std::vector<std::pair<int, float>>& timing
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

    float width() const { return (*textures)[0].width; }
    float height() const { return (*textures)[0].height; }

    Model* model() const { return m_model; }

    /**
     * @fn
     * @brief Function to animate texture on a model by a step.
     * @note It needs to be called every game update for the animation to play, which will likely be called inside a
     * @c while loop.
     */
    void animate();
};