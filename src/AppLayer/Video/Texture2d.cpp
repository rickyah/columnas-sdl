//
//  Texture2d.cpp
//
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "Texture2d.hpp"


Texture2d::Texture2d(SDL_Texture *ptrTexture):
    pSDLTexture(std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(ptrTexture, SDL_DestroyTexture))
{
    SDL_QueryTexture(pSDLTexture.get(), NULL, NULL, &mRealSize.w,  &mRealSize.h);
    mDrawSize = mRealSize;
}

void Texture2d::SetBlendMode(SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(pSDLTexture.get(), mode);
}
