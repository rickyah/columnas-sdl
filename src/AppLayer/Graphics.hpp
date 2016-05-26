//
//  Graphics.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 22/5/16.

#ifndef Graphics_hpp
#define Graphics_hpp

#include <memory>
#include "SDL.h"
#include "Renderer.hpp"


class Graphics
{
public:
    Graphics(int w = 0, int h = 0);
    ~Graphics();
    bool Init(int w = 0, int h = 0);
    void Destroy();
    
    int width() const { return _width; }
    int height() const {return _height; }
    

    std::shared_ptr<Renderer> renderer();
    
private:
    int _width;
    int _height;
    
    std::shared_ptr<SDL_Window> _pSDLWindow;
    
    std::shared_ptr<Renderer> _pRenderer;
    
    Graphics(const Graphics &);
    
    void operator=(const Graphics&);
};




#endif /* Graphics_hpp */
