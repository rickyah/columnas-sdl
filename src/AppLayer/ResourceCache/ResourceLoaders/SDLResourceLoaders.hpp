//
//  TextureLoader.hpp
//
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef TextureLoader_hpp
#define TextureLoader_hpp

#include <SDL2/SDL_surface.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "Logger.hpp"
#include "ResourceLoaderInterfaces.hpp"
#include "ResourcesDefinitions.hpp"

class SDLTextureLoader : public ITextureLoader
{
public:
    SDLTextureLoader(std::weak_ptr<Renderer> ptrRenderer): pRenderer(ptrRenderer) {}
    
    virtual Texture2d* LoadResourceData(const std::string &resourcePath)
    {
        // SDL_image load functions returns surfaces, so we use the renderer to convert them to SDL_Textures
        // Use a unique_ptr so the surface is freed when leaving this method
        auto pSurfaceData = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface *)>(IMG_Load(resourcePath.c_str()),
                                                                              SDL_FreeSurface);
        
        // Generate a placeholder surface on error
        if (NULL == pSurfaceData)
        {
            Logger::log.Error("Couldn't load texture '%s':\n%s\n Loading a placeholder instead",
                    resourcePath.c_str(),
                    IMG_GetError());
            
            pSurfaceData.reset(SDL_CreateRGBSurface(0, 1, 1, 16, 0, 0, 0 , 0));
            SDL_Rect rect = { 0, 0, 1, 1};
            SDL_FillRect(pSurfaceData.get(), &rect, SDL_MapRGB(pSurfaceData->format, 255, 0, 255));
        }
        
        if (auto ptrRenderer = pRenderer.lock())
        {
            return ptrRenderer->CreateTextureFromSurface(pSurfaceData.get());
        }
        
        Logger::log.Error("Renderer has been destroyed. Couldn't create texture %s", resourcePath.c_str());
        
        return nullptr;
    }
    
private:
    std::weak_ptr<Renderer> pRenderer;
};

class SDLMixerSoundLoader : public ISoundLoader
{
public:
    virtual Sound* LoadResourceData(const std::string &resourcePath)
    {
        Mix_Chunk* pChunkData = Mix_LoadWAV(resourcePath.c_str());
        
        if (!pChunkData)
        {
            Logger::log.Error("Couldn't load sound '%s':\n%s", resourcePath.c_str(), Mix_GetError());
            return nullptr;
        }
        
        return new Sound(pChunkData);
    }
};


class SDLMixerMusicLoader : public IMusicLoader
{
public:
    virtual Music* LoadResourceData(const std::string &resourcePath)
    {
        Mix_Music* pMusicData = Mix_LoadMUS(resourcePath.c_str());
        
        if (!pMusicData)
        {
            Logger::log.Error("Couldn't load music '%s':\n%s", resourcePath.c_str(), Mix_GetError());
            return nullptr;
        }
        
        return new Music(pMusicData);
    }
};

#endif /* TextureLoader_hpp */
