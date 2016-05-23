//
//  CGraphics.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 22/5/16.

#ifndef CGraphics_hpp
#define CGraphics_hpp

#include <memory>
#include "SDL.h"

class CRenderer;

class CGraphics
{
public:
    CGraphics(int w, int h);
    ~CGraphics();
    bool init(int w, int h);
    void destroy();
    
    const std::shared_ptr<CRenderer> renderer() const;
    
private:
    std::shared_ptr<SDL_Window> _pSDLWindow;
    
    std::shared_ptr<CRenderer> _pRenderer;
    
    CGraphics(const CGraphics &);
    
    void operator=(const CGraphics&);
};


class CRenderer
{
public:
    CRenderer(const std::shared_ptr<SDL_Window> &pSDLWindow);
    void clear();
    void setColor(int r, int g, int b, int a=255);
    void fillRectangle(int x, int y, int w, int h);
    void present();
    
private:
    std::shared_ptr<SDL_Renderer> _pSDLRenderer;
    
    CRenderer(const CRenderer &);
    void operator=(const CRenderer&);
};


#endif /* CGraphics_hpp */
