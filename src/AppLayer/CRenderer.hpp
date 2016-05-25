//
//  CRenderer.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef CRenderer_hpp
#define CRenderer_hpp

#include <memory>
#include "SDL.h"
#include "SDL_Image.h"
#include "CTexture.hpp"
#include "DataTypes.hpp"

class CRenderer
{
public:
    CRenderer(const std::shared_ptr<SDL_Window> &pSDLWindow);
    void Clear();
    std::shared_ptr<CTexture> LoadTextureFromFile(const char* fileName);
    std::shared_ptr<CTexture> CreateTextureFromSurface(SDL_Surface * pSurface);
    void DrawTextureAt(std::shared_ptr<CTexture> pTexture, Position pos);
    void FillRectangle(Rect rectangle);
    void Present();
    void SetColor(int r, int g, int b, int a=255);
    
private:
    std::shared_ptr<SDL_Renderer> _pSDLRenderer;
    
    CRenderer(const CRenderer &);
    void operator=(const CRenderer&);
};


#endif /* CRenderer_hpp */
