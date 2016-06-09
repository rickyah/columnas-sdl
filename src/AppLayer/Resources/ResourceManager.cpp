//
//  ResourceManager.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#include "ResourceManager.hpp"


ResourceManager::ResourceManager(std::shared_ptr<ITextureLoader> pTextureLoader)
:_pTextureLoader(pTextureLoader)
{
    
}


/// TODO: Add here new template specializations to handle the creation of new resources 
template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<Texture2dResource>(const ResourceId & resourceId) const
{
    return std::make_shared<Texture2dResource>(resourceId, _pTextureLoader);
}
