//
//  ResourcesDefinitions.hpp
//
//
//  Created by Ricardo Amores Hernández on 1/6/16.
//
//

#ifndef ResourcesDefinitions_hpp
#define ResourcesDefinitions_hpp

#include <vector>

#include <SDL2/SDL.h>

#include "Resource.hpp"
#include "ResourceLoaderInterfaces.hpp"

#include "Texture2d.hpp"
#include "Sound.hpp"
#include "Music.hpp"

class Texture2dResource : public Resource
{
public:
    
    Texture2dResource(ResourceId rId, std::weak_ptr<ITextureLoader> pLoader):Resource(rId), pResourceLoader(pLoader)
    {}
    
    Texture2dResource(ResourceId rId, std::shared_ptr<Texture2d> ptrResource):Resource(rId), pResource(ptrResource)
    {}
    
    virtual ~Texture2dResource() {}
    
    std::weak_ptr<Texture2d> resource()
    {
        if (!pResource)
        {
            pResource = std::shared_ptr<Texture2d>(pResourceLoader.lock()->LoadResourceData(name()));
        }
        
        return pResource;
    }

    
protected:
    
private:
    std::shared_ptr<Texture2d> pResource;
    std::weak_ptr<ITextureLoader> pResourceLoader;
};


#include "BitmapFont.hpp"

class BitmapFontResource : public Resource
{
public:
    BitmapFontResource(ResourceId rId, std::weak_ptr<ITextureLoader> pTextureLoader, std::vector<Glyph> glyphData)
    :Resource(rId),
    pTextureResourceLoader(pTextureLoader),
    mGlyphData(glyphData)
    {
    
    }
    
    virtual ~BitmapFontResource()
    {
        pResource.reset();
    }
    
    std::weak_ptr<BitmapFont> resource()
    {
        if (!pResource)
        {
            pResource = std::shared_ptr<BitmapFont>(new BitmapFont(pTextureResourceLoader.lock()->LoadResourceData(name()), mGlyphData));
        }
        
        return pResource;
    }
    
protected:
    std::shared_ptr<BitmapFont> pResource;
    std::weak_ptr<ITextureLoader> pTextureResourceLoader;
    std::vector<Glyph> mGlyphData;
};

class SoundResource : public Resource
{
public:
    SoundResource(ResourceId rId, std::weak_ptr<ISoundLoader> ptrLoader)
    :Resource(rId),
    pResourceLoader(ptrLoader)
    {
    }
    
    virtual ~SoundResource()
    {
        pResource.reset();
    }
    
    std::weak_ptr<Sound>  resource()
    {
        if (!pResource)
        {
            pResource = std::shared_ptr<Sound>(pResourceLoader.lock()->LoadResourceData(name()));
        }
        
        return pResource;
    }
    
protected:
    std::shared_ptr<Sound> pResource;
    std::weak_ptr<ISoundLoader> pResourceLoader;
};

class MusicResource : public Resource
{
public:
    MusicResource(ResourceId rId, std::weak_ptr<IMusicLoader> ptrLoader)
    :Resource(rId),
    pResourceLoader(ptrLoader)
    {
    }
    
    virtual ~MusicResource()
    {
        pResource.reset();
    }
    
    std::weak_ptr<Music> resource()
    {
        if (!pResource)
        {
            pResource = std::shared_ptr<Music>(pResourceLoader.lock()->LoadResourceData(name()));
        }
        
        return pResource;
    }
    
protected:
    std::shared_ptr<Music> pResource;
    std::weak_ptr<IMusicLoader> pResourceLoader;
};

#endif /* ResourcesDefinition_hpps */
