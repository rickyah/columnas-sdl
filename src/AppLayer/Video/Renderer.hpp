//
//  Renderer.hpp
//
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "DataTypes.hpp"
#include "Texture2d.hpp"
#include "BitmapFont.hpp"


/*
 * This class is the responsible of drawing things in the window's screen.
 */
class Renderer
{
public:
    
    explicit Renderer(std::weak_ptr<SDL_Window> ptrSDLWindow);
    ~Renderer();
    
    void Clear();
    
    void SetClipRect(const Rect &clipRec);
    
    void ClearClipRect();

    Size renderSize() const;

    // Text drawing
    void DrawText(const std::string &text, BitmapFont * const pFont, Position pos, float scale = 1);
    void DrawTextCentered(const std::string &text, BitmapFont * const pFont, Position pos, float scale = 1);
    
    // Texture drawing
    void DrawTexture(Texture2d * const pTexture, const Position &pos);
    void DrawTexture(Texture2d * const pTexture, const Position &pos, const Size &drawSize);
    // Clip rect is relative to the DRAW size, not the real size
    void DrawTexture(Texture2d * const pTexture, const Position &pos, const Rect &clipRect);
    void DrawTexture(Texture2d * const pTexture, const Position &pos, const Size &drawSize, const Rect &clipRect);
    
    void SetRenderTargetToTexture(Texture2d * text);
    void SetRenderTargetToScreen();
    
    // Primitive drawing
    void SetColor(int r, int g, int b, int a=255);
    void DrawFillRectangle(Rect rectangle);
    void DrawRectangle(Rect rectangle);
    
    void Present();
    
    // Helper method to create an SDL_Texture from an SDL_Surface, which is required to create the Texture2d
    // The caller is responsible to free the memory from the created Texture2d
    Texture2d * CreateTextureFromSurface(SDL_Surface * pSurface);
    
    // Helper method to create an SDL_Texture
    // The caller is responsible to free the memory from the created Texture2d
    Texture2d * CreateTexture(Uint32 format, int access, int w, int h);
    
private:
    
    Size getTextSize(const std::string &text, BitmapFont * const, float);

    std::shared_ptr<SDL_Renderer> pSDLRenderer;
    
    Renderer(const Renderer &) = delete;
    void operator=(const Renderer&) = delete;
};


#endif /* Renderer_hpp */
