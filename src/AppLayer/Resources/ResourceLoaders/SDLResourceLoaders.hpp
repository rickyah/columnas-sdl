//
//  TextureLoader.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef TextureLoader_hpp
#define TextureLoader_hpp

#include "SDL_surface.h"
#include "SDL_Image.h"
#include "ResourceLoaderInterfaces.hpp"
#include "ResourcesDefinitions.hpp"

class SDLTextureLoader : public ITextureLoader
{
public:
    virtual std::shared_ptr<Texture2d> LoadResourceData(const std::string &resourcePath)
    {
        
        SDL_Surface* pSurfaceData = IMG_Load(resourcePath.c_str());
        
        // Generate a placeholder surface on error
        if (NULL == pSurfaceData)
        {
            pSurfaceData = SDL_CreateRGBSurface(0, 1, 1, 16, 0, 0, 0 , 0);
            SDL_Rect rect = { 0, 0, 1, 1};
            SDL_FillRect(pSurfaceData, &rect, SDL_MapRGB(pSurfaceData->format, 255, 0, 255));
        }
        return std::make_shared<Texture2d>(pSurfaceData);
    }
};


#endif /* TextureLoader_hpp */
