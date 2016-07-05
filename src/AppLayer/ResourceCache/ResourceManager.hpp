//
//  ResourceManager.hpp
//
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <memory>
#include <unordered_map>

#include "Logger.hpp"
#include "ResourceId.hpp"
#include "Resource.hpp"
#include "ResourceLoaderInterfaces.hpp"
#include "ResourcesDefinitions.hpp"

class ResourceManager
{
public:
    ResourceManager(std::shared_ptr<ITextureLoader> ptrTextureLoader,
                    std::shared_ptr<ISoundLoader> ptrSoundLoader,
                    std::shared_ptr<IMusicLoader> ptrMusicLoader);
    
    ~ResourceManager();
    
    template <class T>
    std::weak_ptr<T> Register(std::shared_ptr<T> resource);
    
    template <class T, typename ...Arguments>
    std::weak_ptr<T> Register(const ResourceId &res, Arguments&&... args);
    
    template <class T>
    std::weak_ptr<T> Get(ResourceId res);
    
    void LogReferences();
private:
    
    /*
     * Use template specialization to generate multiple CreateResource<T> functions for each Resource type
     * as each different Resource derived class will probably need a specific ResourceLoader class
     * to load the resource from the physical media
     *
     * Define the specialization in the file ResourceManager.cpp
     */
    template <class T, typename ...Arguments>
    std::shared_ptr<Resource> CreateResource(const ResourceId & resourceId, Arguments&&... args) const;
    
    ResourceManager(const ResourceManager &);
    ResourceManager & operator=(const ResourceManager &);
    
    std::unordered_map<uint32_t, std::shared_ptr<Resource>> mIdToResourceMapping;
    
    std::shared_ptr<ITextureLoader> pTextureLoader;
    std::shared_ptr<ISoundLoader> pSoundLoader;
    std::shared_ptr<IMusicLoader> pMusicLoader;
};


template <class T>
std::weak_ptr<T> ResourceManager::Register(std::shared_ptr<T> resource)
{
    mIdToResourceMapping[resource->id()] = resource;
    
    return resource;
}

// template method implementations must be in the header
template <class T, typename... Arguments>
std::weak_ptr<T> ResourceManager::Register(const ResourceId &res, Arguments&&... args)
{
    auto resource = CreateResource<T>(res, std::forward<Arguments>(args)...);
    
    mIdToResourceMapping[res.id] = resource;
    
    Logger::log.Info("Registered resource %s", resource->name().c_str());

    return std::static_pointer_cast<T>(resource);
}

template <class T>
std::weak_ptr<T> ResourceManager::Get(ResourceId res)
{
    auto resource = mIdToResourceMapping[res.id];
    Logger::log.Info("Get resource %s, refcount: %ld", resource->name().c_str(), resource.use_count());
    return std::static_pointer_cast<T>(resource);
}


#endif /* ResourceManager_hpp */
