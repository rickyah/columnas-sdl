//
//  Window.cpp
//
//
//  Created by Ricardo Amores Hernández on 22/5/16.
//
//

#include "Window.hpp"

Window::Window(int w, int h, bool useFullScreen): mWidth(0), mHeight(0)
{
    this->Init(w,h, useFullScreen);
}

Window::~Window()
{
    this->Destroy();
}

bool Window::Init(int w, int h, bool useFullScreen)
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

    int windowFlags = 0;
    if (useFullScreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }
    
    pSDLWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow(NULL,
                                                              SDL_WINDOWPOS_UNDEFINED,
                                                              SDL_WINDOWPOS_UNDEFINED,
                                                              mWidth,
                                                              mHeight,
                                                              windowFlags),
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

void Window::Destroy()
{
    pRenderer.reset();
    pSDLWindow.reset();
}

const std::string& Window::title() const
{
    return mWindowTitle;
}

void Window::title(const std::string& title)
{
    mWindowTitle = title;
    SDL_SetWindowTitle(pSDLWindow.get(), mWindowTitle.c_str());
}


