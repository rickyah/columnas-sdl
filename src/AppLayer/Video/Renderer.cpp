//
//  Renderer.cpp
//
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "Renderer.hpp"


Renderer::Renderer(std::weak_ptr<SDL_Window> ptrSDLWindow)
{
    pSDLRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(ptrSDLWindow.lock().get(), -1, SDL_RENDERER_ACCELERATED),
                                                 SDL_DestroyRenderer);
}

Renderer::~Renderer()
{
    if(pSDLRenderer) pSDLRenderer.reset();
}

Size Renderer::renderSize() const
{
    Size renderSize;
    SDL_GetRendererOutputSize(pSDLRenderer.get(), &renderSize.w, &renderSize.h);
    
    return renderSize;
}
void Renderer::Clear()
{
    SDL_RenderClear(pSDLRenderer.get());
}

Texture2d* Renderer::CreateTextureFromSurface(SDL_Surface * pSurface)
{
    return new Texture2d(SDL_CreateTextureFromSurface(pSDLRenderer.get(), pSurface));
}

Texture2d* Renderer::CreateTexture(Uint32 format, int access, int w, int h)
{
    return new Texture2d(SDL_CreateTexture(pSDLRenderer.get(), format, access, w, h));
}

void Renderer::ClearClipRect()
{
    SDL_RenderSetClipRect(pSDLRenderer.get(), NULL);
}

void Renderer::SetClipRect(const Rect &clipRec)
{
    SDL_Rect sdlRect = {clipRec.position.x, clipRec.position.y, clipRec.size.w, clipRec.size.h};
    SDL_RenderSetClipRect(pSDLRenderer.get(), &sdlRect);
}

void Renderer::DrawTexture(Texture2d * const pTexture, const Position& pos)
{
    if (!pTexture) return;
    
    DrawTexture(pTexture, pos, pTexture->drawSize());
}

void Renderer::DrawTexture(Texture2d * const pTexture, const Position &pos, const Size &drawSize)
{
    if (!pTexture) return;
    
    SDL_Rect drawSizeRect = {pos.x, pos.y, drawSize.w, drawSize.h};
    SDL_RenderCopy(pSDLRenderer.get(), pTexture->pSDLTexture.get(), NULL, &drawSizeRect);
}


void Renderer::DrawTexture(Texture2d * const pTexture, const Position &pos, const Rect &clipRect)
{
    if (!pTexture) return;
    
    DrawTexture(pTexture, pos, pTexture->drawSize(), clipRect);
}

void Renderer::DrawTexture(Texture2d * const pTexture, const Position &pos, const Size &drawSize, const Rect &clipRect)
{
    if (!pTexture) return;
    
    auto realSize = pTexture->realSize();
    
    // The clip rect is based on the texture's draw size, but SDL needs a rect with the coordinates and size
    // of the real size of the texture, so we just multiply the clipRect by the relation beween the real size
    // and the drawing size
    float wFactor = static_cast<float>(realSize.w)/static_cast<float>(drawSize.w);
    float hFactor = static_cast<float>(realSize.h)/static_cast<float>(drawSize.h);
    
    SDL_Rect drawSizeRectSDL = {
        pos.x,
        pos.y,
        clipRect.size.w,
        clipRect.size.h
    };
    
    SDL_Rect clipRectSDL = {
        static_cast<int>(clipRect.position.x * wFactor),
        static_cast<int>(clipRect.position.y * hFactor),
        static_cast<int>(clipRect.size.w * wFactor),
        static_cast<int>(clipRect.size.h *wFactor)
    };
    
    auto color = pTexture->colorTint();
    SDL_SetTextureColorMod(pTexture->pSDLTexture.get(), color.r, color.g, color.b);
    
    SDL_RenderCopy(pSDLRenderer.get(), pTexture->pSDLTexture.get(), &clipRectSDL, &drawSizeRectSDL);
}

void Renderer::DrawFillRectangle(Rect rectangle)
{
    SDL_Rect rect = {rectangle.position.x, rectangle.position.y, rectangle.size.w, rectangle.size.h};
    
    SDL_RenderFillRect(pSDLRenderer.get(), &rect);
}

void Renderer::DrawRectangle(Rect rectangle)
{
    SDL_Rect rect = {rectangle.position.x, rectangle.position.y, rectangle.size.w, rectangle.size.h};
    
    SDL_RenderDrawRect(pSDLRenderer.get(), &rect);
}

void Renderer::DrawText(const std::string &text, BitmapFont* const pFont, Position pos, float scale)
{
    static Rect clipRect;
    static Size drawSize;
    
    if (!pFont) return;
    
    auto pTexture = pFont->fontTexture();
    
    if (!pTexture) return;
    
    drawSize = pTexture->drawSize();
    
    drawSize.w *= scale;
    drawSize.h *= scale;
    
    for(char c: text)
    {
        auto glyph = pFont->FindGlyphForChar(c);
        
        clipRect.position.x = glyph.x;
        clipRect.position.y = glyph.y;
        clipRect.size.w = glyph.width;
        clipRect.size.h = glyph.height;
        
        pos.x += glyph.xoffset * scale;
        pos.y += (glyph.yoffset * scale);
        
        
        SDL_Rect drawSizeRectSDL = {
            pos.x,
            pos.y,
            static_cast<int>(clipRect.size.w * scale),
            static_cast<int>(clipRect.size.h * scale)
        };
        
        SDL_Rect clipRectSDL = {
            static_cast<int>(clipRect.position.x),
            static_cast<int>(clipRect.position.y),
            static_cast<int>(clipRect.size.w),
            static_cast<int>(clipRect.size.h)
        };
        
        auto color = pTexture->colorTint();
        SDL_SetTextureColorMod(pTexture->pSDLTexture.get(), color.r, color.g, color.b);
        SDL_RenderCopy(pSDLRenderer.get(), pTexture->pSDLTexture.get(), &clipRectSDL, &drawSizeRectSDL);
        
        pos.x -= glyph.xoffset * scale;
        pos.y -= (glyph.yoffset * scale);
        pos.x += glyph.width * scale;
    }
}

Size Renderer::getTextSize(const std::string &text, BitmapFont *pFont, float scale)
{
    Size textSize;
    
    for(char c: text)
    {
        auto glyph = pFont->FindGlyphForChar(c);
        textSize.w += glyph.width * scale;
        textSize.h = fmax(textSize.h, glyph.height*scale);
    }
    
    return textSize;
}

void Renderer::DrawTextCentered(const std::string &text, BitmapFont * const pFont, Position pos, float scale)
{
    if (!pFont) return;
    
    Size textureSize = getTextSize(text, pFont, scale);

    // Just draw the text offseting the position by half the total size
    pos.x -= static_cast<int>(textureSize.w/2);
    pos.y -= static_cast<int>(textureSize.h/2);
    
    DrawText(text, pFont, pos);
}

void Renderer::Present()
{
    SDL_RenderPresent(pSDLRenderer.get());
}

void Renderer::SetColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(pSDLRenderer.get(), r, g, b, a);
}

void Renderer::SetRenderTargetToTexture(Texture2d * text)
{
    SDL_SetRenderTarget(pSDLRenderer.get(), text->pSDLTexture.get());
}

void Renderer::SetRenderTargetToScreen()
{
    SDL_SetRenderTarget(pSDLRenderer.get(), NULL);
}

