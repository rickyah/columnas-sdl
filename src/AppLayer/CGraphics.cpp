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
    this->Init(w,h);
}

CGraphics::~CGraphics()
{
    this->Destroy();
}

bool CGraphics::Init(int w, int h)
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
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

void CGraphics::Destroy()
{
    _pSDLWindow.reset();
    _pRenderer.reset();
}

const std::shared_ptr<CRenderer> CGraphics::renderer() const
{
    return _pRenderer;
}
