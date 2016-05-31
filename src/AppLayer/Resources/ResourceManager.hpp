//
//  ResourceManager.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <memory>
#include <unordered_map>

#include "ResourceId.hpp"
#include "Resource.hpp"
#include "ResourceLoaderInterfaces.hpp"
#include "ResourcesDefinitions.hpp"

class ResourceManager
{
public:
    ResourceManager(std::shared_ptr<ITextureLoader> pTextureLoader);
    
    template <class T> std::shared_ptr<T> Register(const ResourceId &res);
    
    template <class T> std::shared_ptr<T> Get(ResourceId res);
    
private:
    
    /*
     * Use template specialization to generate multiple CreateResource<T> functions for each Resource type
     * as each different Resource derived class will probably need a specific ResourceLoader class
     * to load the resource from the physical media
     *
     * Define the specialization in the file ResourceManager.cpp
     */
    template <class T> std::shared_ptr<Resource> CreateResource(const ResourceId & resourceId) const;
    
    ResourceManager(const ResourceManager &);
    ResourceManager & operator=(const ResourceManager &);
    
    std::unordered_map<uint32_t, std::shared_ptr<Resource>> _idToResourceMapping;
    
    std::shared_ptr<ITextureLoader> _pTextureLoader;
};

// template method implementations must be in the header
template <class T>
std::shared_ptr<T> ResourceManager::Register(const ResourceId &res)
{
    auto resource = CreateResource<T>(res);
    _idToResourceMapping[res.id] = resource;
    
    return std::static_pointer_cast<T>(resource);
}

template <class T>
std::shared_ptr<T> ResourceManager::Get(ResourceId res)
{
    return std::static_pointer_cast<T>(_idToResourceMapping[res.id]);
}


#endif /* ResourceManager_hpp */
