//
//  Texture2d.hpp
//
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef Texture_hpp
#define Texture_hpp

#include <memory>
#include <SDL2/SDL.h>
#include "DataTypes.hpp"

class Texture2d
{
    // The renderer needs access to the raw pointer
    friend class Renderer;
    
public:
    Texture2d(SDL_Texture *pTexture);
    
    const Size realSize() const { return mRealSize; }
    const Size drawSize() const { return mDrawSize; }
    void drawSize(Size newSize) { mDrawSize = newSize; }
    
    void SetBlendMode(SDL_BlendMode mode);
    
    Texture2d(const Texture2d &) = delete;
    Texture2d& operator=(const Texture2d &) = delete;
    
    Color colorTint() const { return mRGBColorTint; }
    Color colorTint(Color rgbColor) { auto previous = mRGBColorTint; mRGBColorTint = rgbColor; return previous; }
private:

    Color mRGBColorTint = {255,255,255};
    Size mDrawSize;
    Size mRealSize;
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> pSDLTexture;
};

#endif /* Texture_hpp */
