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
    pTextureData.reset();
    pSurfaceData.reset();
}


Texture2d::Texture2d(SDL_Surface *pSurface):
    mRealSize(Size(pSurface->w, pSurface->h)),
    mDrawSize(Size(pSurface->w, pSurface->h)),
    pSurfaceData(std::shared_ptr<SDL_Surface>(pSurface, SDL_FreeSurface))
{
}

Texture2d::Texture2d(SDL_Texture *pTexture, Size textureSize):
    mRealSize(textureSize),
    mDrawSize(textureSize),
    pTextureData(std::shared_ptr<SDL_Texture>(pTexture, SDL_DestroyTexture))
{
}


Texture2d::Texture2d(SDL_Texture *pTexture):
    pTextureData(std::shared_ptr<SDL_Texture>(pTexture, SDL_DestroyTexture))
{
    SDL_QueryTexture(pTexture, NULL, NULL, &mRealSize.w,  &mRealSize.h);
    mDrawSize = mRealSize;
}