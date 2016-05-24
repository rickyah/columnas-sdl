//
//  CGraphics.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 22/5/16.
//
//

#include "CGraphics.hpp"

CGraphics::CGraphics(int w, int h): _width(0), _height(0)
{
    this->init(w,h);
}

CGraphics::~CGraphics()
{
    this->destroy();
}

bool CGraphics::init(int w, int h)
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    if (w <= 0 || h <= 0 || w > displayMode.w || h > displayMode.h)
    {
        w = displayMode.w;
        h = displayMode.h;
    }
    
    _width = w;
    _height = h;

    _pSDLWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow(NULL,0, 0, _width, _height, SDL_WINDOW_FULLSCREEN),
                                              SDL_DestroyWindow);
 
    _pRenderer = std::shared_ptr<CRenderer>(new CRenderer(_pSDLWindow));
}

void CGraphics::destroy()
{
    _pSDLWindow.reset();
    _pRenderer.reset();
}

const std::shared_ptr<CRenderer> CGraphics::renderer() const
{
    return _pRenderer;
}

CRenderer::CRenderer(const std::shared_ptr<SDL_Window> &pSDLWindow)
{
    _pSDLRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(pSDLWindow.get(), -1, 0),
                                                  SDL_DestroyRenderer);
}

void CRenderer::clear()
{
    SDL_RenderClear(_pSDLRenderer.get());
}

void CRenderer::setColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(_pSDLRenderer.get(), r, g, b, a);
}

void CRenderer::present()
{
    SDL_RenderPresent(_pSDLRenderer.get());
}

void CRenderer::fillRectangle(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    
    SDL_RenderFillRect(_pSDLRenderer.get(), &rect);
}
