//
//  Graphics.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 22/5/16.
//
//

#include "Graphics.hpp"

Graphics::Graphics(int w, int h): mWidth(0), mHeight(0)
{
    this->Init(w,h);
}

Graphics::~Graphics()
{
    this->Destroy();
}

bool Graphics::Init(int w, int h)
{
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }
    
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    if (w <= 0 || h <= 0 || w > displayMode.w || h > displayMode.h)
    {
        w = displayMode.w;
        h = displayMode.h;
    }
    
    mWidth = w;
    mHeight = h;

    pSDLWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow(NULL,0, 0, mWidth, mHeight, SDL_WINDOW_FULLSCREEN),
                                              SDL_DestroyWindow);
 
    if(!pSDLWindow)
    {
        return false;
    }
    
    pRenderer = std::shared_ptr<Renderer>(new Renderer(pSDLWindow));
    
    if (!pRenderer)
    {
        return false;
    }
    
    return true;
}

void Graphics::Destroy()
{
    pSDLWindow.reset();
    pRenderer.reset();
}

