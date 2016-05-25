//
//  CTexture.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef CTexture_hpp
#define CTexture_hpp

#include <memory>
#include "SDL.h"
#include "DataTypes.hpp"

class CTexture
{
public:
    CTexture(SDL_Texture *pTexture);
    CTexture(SDL_Texture *pTexture, Size size);
    ~CTexture() {};
    const Size realSize() const { return _realSize; }
    const Size drawSize() const { return _drawSize; }
    void drawSize(Size newSize) { _drawSize = newSize; }
    const std::shared_ptr<SDL_Texture> textureData() const { return _pTextureData; }
private:
    Size _drawSize;
    Size _realSize;
    std::shared_ptr<SDL_Texture> _pTextureData;
};

#endif /* CTexture_hpp */