//
//  Graphics.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 22/5/16.
//
//

#include "Graphics.hpp"

Graphics::Graphics(int w, int h): _width(0), _height(0)
{
    this->Init(w,h);
}

Graphics::~Graphics()
{
    this->Destroy();
}

bool Graphics::Init(int w, int h)
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
 
    _pRenderer = std::shared_ptr<Renderer>(new Renderer(_pSDLWindow));
}

void Graphics::Destroy()
{
    _pSDLWindow.reset();
    _pRenderer.reset();
}

std::shared_ptr<Renderer> Graphics::renderer()
{
    return _pRenderer;
}
