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
    explicit Graphics(int w = 0, int h = 0);
    ~Graphics();
    bool Init(int w = 0, int h = 0);
    void Destroy();
    
    int width() const { return mWidth; }
    int height() const {return mHeight; }
    

    std::shared_ptr<Renderer> renderer() { return pRenderer; }
    
private:
    int mWidth;
    int mHeight;
    
    std::shared_ptr<SDL_Window> pSDLWindow;
    
    std::shared_ptr<Renderer> pRenderer;
    
    Graphics(const Graphics &);
    
    void operator=(const Graphics&);
};




#endif /* Graphics_hpp */
