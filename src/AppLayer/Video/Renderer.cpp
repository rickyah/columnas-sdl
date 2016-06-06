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
    pSDLRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(pSDLWindow.get(), -1, 0),
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

void Renderer::DrawTextureAt(std::shared_ptr<Texture2d> pTexture, Position pos)
{
    CreateHardwareTexture(pTexture);
    
    SDL_Rect drawSize = {pos.x, pos.y, pTexture->drawSize().w, pTexture->drawSize().h};
    SDL_RenderCopy(pSDLRenderer.get(), pTexture->textureData().get(), NULL, &drawSize);
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

