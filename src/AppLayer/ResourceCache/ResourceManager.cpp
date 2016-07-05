//
//  ResourceManager.cpp
//
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#include "ResourceManager.hpp"

ResourceManager::ResourceManager(std::shared_ptr<ITextureLoader> ptrTextureLoader,
                                 std::shared_ptr<ISoundLoader> ptrSoundLoader,
                                 std::shared_ptr<IMusicLoader> ptrMusicLoader)
:pTextureLoader(ptrTextureLoader), pSoundLoader(ptrSoundLoader), pMusicLoader(ptrMusicLoader)
{
    
}

ResourceManager::~ResourceManager()
{
    Logger::log.Info("cleaning resources");
    LogReferences();
    mIdToResourceMapping.clear();
}

void ResourceManager::LogReferences()
{
    // Need to use a ref to the shared_ptr or we will increase the refcount!
    Logger::log.Info("\nResourceManager::LogReferences()");
    for(const auto &res: mIdToResourceMapping)
    {
        Logger::log.Info("\tresource %s, refcount: %ld", res.second->name().c_str(), res.second.use_count());
    }
}
/// TODO: Add here new template specializations to handle the creation of new resources
template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<Texture2dResource>(const ResourceId & resourceId) const
{
    return std::make_shared<Texture2dResource>(resourceId, pTextureLoader);
}

// TODO: specializing for all possible parameters does not scale and I could manage to do a better argument
// forwarding, need to investigate more about this
template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<BitmapFontResource, const std::vector<Glyph> &>
    (
     const ResourceId & resourceId,
     const std::vector<Glyph>& glyphData
     ) const
{
    return std::make_shared<BitmapFontResource>(resourceId,
                                                pTextureLoader,
                                                glyphData);
}

template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<BitmapFontResource, std::vector<Glyph> &>
(
 const ResourceId & resourceId,
 std::vector<Glyph>& glyphData
 ) const
{
    return std::make_shared<BitmapFontResource>(resourceId,
                                                pTextureLoader,
                                                glyphData);
}


template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<BitmapFontResource, std::vector<Glyph>>
(
 const ResourceId & resourceId,
 std::vector<Glyph>&& glyphData
 ) const
{
    return std::make_shared<BitmapFontResource>(resourceId,
                                                pTextureLoader,
                                                glyphData);
}

template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<SoundResource>(const ResourceId & resourceId) const
{
    return std::make_shared<SoundResource>(resourceId, pSoundLoader);
}

template <>
std::shared_ptr<Resource> ResourceManager::CreateResource<MusicResource>(const ResourceId & resourceId) const
{
    return std::make_shared<MusicResource>(resourceId, pMusicLoader);
}


