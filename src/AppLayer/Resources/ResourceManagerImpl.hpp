//
//  ResourceManagerImpl.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 1/6/16.
//
//

#ifndef ResourceManagerImpl_h
#define ResourceManagerImpl_h

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


#endif /* ResourceManagerImpl_h */
