/** @file */

#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <string_view>
#include <mutex>

#include "raylib.h"
#include "box2d/box2d.h"

#include "constants.h"

/**
 * @class
 * @brief Game manager class to manage game resources.
 */
class ResourceManager
{
private:

    /**
     * @name Private Key
     * @brief Key to prevent further instantiation after Singleton instance.
     */
    struct PrivateKey { explicit PrivateKey() = default; };

    static std::unique_ptr<ResourceManager> m_instance;
    static std::mutex locker;

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

    /** @} */

    /**
     * @brief Delete copy constructors/assignment operators to disallow copying between resource managers.
     * @{
     */

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /** @} */

    /**
     * @param 
     * @param path_sv Formatted path to textures. Paths are formatted (follow a certain regular expression) to allow
     * animations.
     */
    std::vector<Texture> load_texture_resource(const int count, const std::string_view path_sv);
    
    Shader load_shader_resource(const std::string_view path_sv);
    Shader load_shader_resource_fs(const std::string_view path_sv);

public:

    /**
     * Constructor and destructor are made public to let @c std::unique_ptr get accessed to them, but they're intended
     * to be private so as to prevent user from instantiating another resource manager after the Singleton instance.
     * 
     * To prevent user from instantiating, the @ref PrivateKey private structure was created to only allow
     * instantiation from within this class, guaranteeing Singleton instance.
     * @{
     */

    explicit ResourceManager(PrivateKey);
    ~ResourceManager();

    /** @} */

    /**
     * @fn
     * @brief ResourceManager static instance.
     * 
     * ResourceManager follows a design pattern known as the Singleton pattern, in which only 1 instance of a class is
     * allowed to exist at any given time.
     */
    static ResourceManager* instance();

    static void destroy();

    inline b2WorldId world_id() { return m_world_id; }

    inline std::vector<Texture>* texture_resource(const TextureResource texture_resource)
    {
        return &texture_resources[static_cast<int>(texture_resource)];
    }

    inline Shader* shader_resource(const ShaderResource shader_resource)
    {
        return &shader_resources[static_cast<int>(shader_resource)];
    }

    inline Model* model_resource(const ModelResource model_resource)
    {
        return &model_resources[static_cast<int>(model_resource)];
    }
};
