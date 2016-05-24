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
    CGraphics(int w = 0, int h = 0);
    ~CGraphics();
    bool init(int w = 0, int h = 0);
    void destroy();
    
    int width() const { return _width; }
    int height() const {return _height; }
    
    const std::shared_ptr<CRenderer> renderer() const;
    
private:
    int _width;
    int _height;
    
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
