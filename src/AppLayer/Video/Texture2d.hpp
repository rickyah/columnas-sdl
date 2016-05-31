//
//  Texture2d.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef Texture_hpp
#define Texture_hpp

#include <memory>
#include "SDL.h"
#include "DataTypes.hpp"

class Texture2d
{
    friend class Renderer;
public:
    Texture2d(SDL_Surface *pSurface);
    Texture2d(SDL_Texture *pTexture);
    Texture2d(SDL_Texture *pTexture, Size textureSize);
    
    ~Texture2d();
    const Size realSize() const { return _realSize; }
    const Size drawSize() const { return _drawSize; }
    void drawSize(Size newSize) { _drawSize = newSize; }
   
    const std::shared_ptr<SDL_Surface> surfaceData() const { return _pSurfaceData; }
    const std::shared_ptr<SDL_Texture> textureData() const { return _pTextureData; }
    
    
    std::shared_ptr<Texture2d> LoadTextureFromFile(const char* fileName);
    std::shared_ptr<Texture2d> CreateTextureFromSurface(SDL_Surface * pSurface);
    
private:
    Size _drawSize;
    Size _realSize;
    std::shared_ptr<SDL_Surface> _pSurfaceData;
    std::shared_ptr<SDL_Texture> _pTextureData;
};

#endif /* Texture_hpp */
