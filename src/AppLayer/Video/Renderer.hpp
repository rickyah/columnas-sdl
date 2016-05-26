//
//  Renderer.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <memory>
#include "SDL.h"
#include "SDL_Image.h"
#include "Texture.hpp"
#include "DataTypes.hpp"

class Renderer
{
public:
    Renderer(const std::shared_ptr<SDL_Window> &pSDLWindow);
    void Clear();
    std::shared_ptr<Texture> LoadTextureFromFile(const char* fileName);
    std::shared_ptr<Texture> CreateTextureFromSurface(SDL_Surface * pSurface);
    void DrawTextureAt(std::shared_ptr<Texture> pTexture, Position pos);
    void FillRectangle(Rect rectangle);
    void Present();
    void SetColor(int r, int g, int b, int a=255);
    
private:
    std::shared_ptr<SDL_Renderer> _pSDLRenderer;
    
    Renderer(const Renderer &);
    void operator=(const Renderer&);
};


#endif /* Renderer_hpp */
