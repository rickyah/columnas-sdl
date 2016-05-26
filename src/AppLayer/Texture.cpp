//
//  CTexture.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "Texture.hpp"

Texture::Texture(SDL_Texture *pTexture, Size size):
    _realSize(size),
    _drawSize(size),
    _pTextureData(std::shared_ptr<SDL_Texture>(pTexture, SDL_DestroyTexture))
{
    
}

Texture::Texture(SDL_Texture *pTexture):
    _pTextureData(std::shared_ptr<SDL_Texture>(pTexture, SDL_DestroyTexture))
{
    SDL_QueryTexture(pTexture, NULL, NULL, &_realSize.w,  &_realSize.h);
    _drawSize = _realSize;
}