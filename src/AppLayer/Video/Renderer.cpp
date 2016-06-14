//
//  Renderer.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "Renderer.hpp"


Renderer::Renderer(const std::shared_ptr<SDL_Window> &pSDLWindow)
{
    pSDLRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(pSDLWindow.get(), -1, SDL_RENDERER_ACCELERATED),
                                                  SDL_DestroyRenderer);
}

void Renderer::Clear()
{
    SDL_RenderClear(pSDLRenderer.get());
}

std::shared_ptr<Texture2d> Renderer::CreateTextureFromSurface(SDL_Surface * pSurface)
{

    Size textureSize = {pSurface->w, pSurface->h};
    
    return std::make_shared<Texture2d>(SDL_CreateTexture(pSDLRenderer.get(),
                                                       SDL_PIXELFORMAT_UNKNOWN,
                                                       SDL_TEXTUREACCESS_STATIC,
                                                       pSurface->w,
                                                       pSurface->h),
                                     textureSize);
}

void Renderer::CreateHardwareTexture(std::shared_ptr<Texture2d> texture) const
{
    auto textureData = texture->textureData();
    if(!textureData)
    {
        textureData = std::shared_ptr<SDL_Texture>(
            SDL_CreateTextureFromSurface(pSDLRenderer.get(), texture->surfaceData().get()),
            SDL_DestroyTexture);
        texture->textureData(textureData);
    }
    

}


void Renderer::DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position& pos)
{
    CreateHardwareTexture(pTexture);
    
    DrawTexture(pTexture, pos, pTexture->drawSize());
}

void Renderer::DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos, const Size &drawSize)
{
    CreateHardwareTexture(pTexture);
    
    SDL_Rect drawSizeRect = {pos.x, pos.y, drawSize.w, drawSize.h};
    SDL_RenderCopy(pSDLRenderer.get(), pTexture->textureData().get(), NULL, &drawSizeRect);
}


void Renderer::DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos, const Rect &clipRect)
{
    CreateHardwareTexture(pTexture);
    
    DrawTexture(pTexture, pos, pTexture->drawSize(), clipRect);
}

void Renderer::DrawTexture(std::shared_ptr<Texture2d> pTexture, const Position &pos, const Size &drawSize, const Rect &clipRect)
{
    CreateHardwareTexture(pTexture);
    
    auto realSize = pTexture->realSize();
    
    // The clip rect is based on the draw size of the texture, but SDL needs a rect with the coordinates and size
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
    
    SDL_RenderCopy(pSDLRenderer.get(), pTexture->textureData().get(), &clipRectSDL, &drawSizeRectSDL);
}

void Renderer::FillRectangle(Rect rectangle)
{
    SDL_Rect rect = {rectangle.position.x, rectangle.position.y, rectangle.size.w, rectangle.size.h};
    
    SDL_RenderFillRect(pSDLRenderer.get(), &rect);
}

void Renderer::Present()
{
    SDL_RenderPresent(pSDLRenderer.get());
}

void Renderer::SetColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(pSDLRenderer.get(), r, g, b, a);
}

