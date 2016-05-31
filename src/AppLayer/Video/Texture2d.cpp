//
//  Texture2d.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "Texture2d.hpp"


Texture2d::~Texture2d()
{
    _pTextureData.reset();
    _pSurfaceData.reset();
}


Texture2d::Texture2d(SDL_Surface *pSurface):
    _realSize(Size(pSurface->w, pSurface->h)),
    _drawSize(Size(pSurface->w, pSurface->h)),
    _pSurfaceData(std::shared_ptr<SDL_Surface>(pSurface, SDL_FreeSurface))
{
}

Texture2d::Texture2d(SDL_Texture *pTexture, Size textureSize):
    _realSize(textureSize),
    _drawSize(textureSize),
    _pTextureData(std::shared_ptr<SDL_Texture>(pTexture, SDL_DestroyTexture))
{
}


Texture2d::Texture2d(SDL_Texture *pTexture):
    _pTextureData(std::shared_ptr<SDL_Texture>(pTexture, SDL_DestroyTexture))
{
    SDL_QueryTexture(pTexture, NULL, NULL, &_realSize.w,  &_realSize.h);
    _drawSize = _realSize;
}