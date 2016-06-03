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
    const Size realSize() const { return mRealSize; }
    const Size drawSize() const { return mDrawSize; }
    void drawSize(Size newSize) { mDrawSize = newSize; }
   
    const std::shared_ptr<SDL_Surface> surfaceData() const { return pSurfaceData; }
    const std::shared_ptr<SDL_Texture> textureData() const { return pTextureData; }
    
    
    std::shared_ptr<Texture2d> LoadTextureFromFile(const char* fileName);
    std::shared_ptr<Texture2d> CreateTextureFromSurface(SDL_Surface * pSurface);
    
private:
    Size mDrawSize;
    Size mRealSize;
    std::shared_ptr<SDL_Surface> pSurfaceData;
    std::shared_ptr<SDL_Texture> pTextureData;
};

#endif /* Texture_hpp */
