//
//  CRenderer.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "CRenderer.hpp"


CRenderer::CRenderer(const std::shared_ptr<SDL_Window> &pSDLWindow)
{
    _pSDLRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(pSDLWindow.get(), -1, 0),
                                                  SDL_DestroyRenderer);
}

void CRenderer::Clear()
{
    SDL_RenderClear(_pSDLRenderer.get());
}

std::shared_ptr<CTexture> CRenderer::CreateTextureFromSurface(SDL_Surface * pSurface)
{
    return std::shared_ptr<CTexture>(new CTexture(SDL_CreateTexture(_pSDLRenderer.get(),
                                                                    SDL_PIXELFORMAT_UNKNOWN,
                                                                    SDL_TEXTUREACCESS_STATIC,
                                                                    pSurface->w,
                                                                    pSurface->h),
                                                  {pSurface->w, pSurface->h}));
}

void CRenderer::DrawTextureAt(std::shared_ptr<CTexture> pTexture, Position pos)
{
    SDL_Rect drawSize = {pos.x, pos.y, pTexture->drawSize().w, pTexture->drawSize().h};
    SDL_RenderCopy(_pSDLRenderer.get(), pTexture->textureData().get(), NULL, &drawSize);
}


std::shared_ptr<CTexture> CRenderer::LoadTextureFromFile(const char* fileName)
{
    return std::shared_ptr<CTexture>(new CTexture(IMG_LoadTexture(_pSDLRenderer.get(), fileName)));
}


void CRenderer::FillRectangle(Rect rectangle)
{
    SDL_Rect rect = {rectangle.position.x, rectangle.position.y, rectangle.size.w, rectangle.size.h};
    
    SDL_RenderFillRect(_pSDLRenderer.get(), &rect);
}

void CRenderer::Present()
{
    SDL_RenderPresent(_pSDLRenderer.get());
}

void CRenderer::SetColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(_pSDLRenderer.get(), r, g, b, a);
}

