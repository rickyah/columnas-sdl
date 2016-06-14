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
#include "DataTypes.hpp"
#include "Texture2d.hpp"

class Renderer
{
public:
    Renderer(const std::shared_ptr<SDL_Window> &pSDLWindow);
    void Clear();
    std::shared_ptr<Texture2d> CreateTextureFromSurface(SDL_Surface * pSurface);
    void CreateHardwareTexture(std::shared_ptr<Texture2d> texture) const;
    void DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos);
    void DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos, const Size &drawSize);
    
    // Clip rect is relative to the DRAW size, not the real size
    void DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos, const Rect &clipRect);
    void DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos, const Size &drawSize, const Rect &clipRect);
    void FillRectangle(Rect rectangle);
    void Present();
    void SetColor(int r, int g, int b, int a=255);
    
private:
    std::shared_ptr<SDL_Renderer> pSDLRenderer;
    
    Renderer(const Renderer &);
    void operator=(const Renderer&);
};


#endif /* Renderer_hpp */
