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
    _pSDLRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(pSDLWindow.get(), -1, 0),
                                                  SDL_DestroyRenderer);
}

void Renderer::Clear()
{
    SDL_RenderClear(_pSDLRenderer.get());
}

std::shared_ptr<Texture> Renderer::CreateTextureFromSurface(SDL_Surface * pSurface)
{

    Size textureSize = {pSurface->w, pSurface->h};
    
    return std::make_shared<Texture>(SDL_CreateTexture(_pSDLRenderer.get(),
                                                       SDL_PIXELFORMAT_UNKNOWN,
                                                       SDL_TEXTUREACCESS_STATIC,
                                                       pSurface->w,
                                                       pSurface->h),
                                     textureSize);
}

void Renderer::DrawTextureAt(std::shared_ptr<Texture> pTexture, Position pos)
{
    SDL_Rect drawSize = {pos.x, pos.y, pTexture->drawSize().w, pTexture->drawSize().h};
    SDL_RenderCopy(_pSDLRenderer.get(), pTexture->textureData().get(), NULL, &drawSize);
}


std::shared_ptr<Texture> Renderer::LoadTextureFromFile(const char* fileName)
{
    return std::make_shared<Texture>(IMG_LoadTexture(_pSDLRenderer.get(), fileName));
}


void Renderer::FillRectangle(Rect rectangle)
{
    SDL_Rect rect = {rectangle.position.x, rectangle.position.y, rectangle.size.w, rectangle.size.h};
    
    SDL_RenderFillRect(_pSDLRenderer.get(), &rect);
}

void Renderer::Present()
{
    SDL_RenderPresent(_pSDLRenderer.get());
}

void Renderer::SetColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(_pSDLRenderer.get(), r, g, b, a);
}

