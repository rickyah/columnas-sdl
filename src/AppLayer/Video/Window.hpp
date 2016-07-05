//
//  Window.hpp
//
//
//  Created by Ricardo Amores Hernández on 22/5/16.

#ifndef Window_hpp
#define Window_hpp

#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include "Renderer.hpp"


class Window
{
    // Needed for App to access some internal data for initialization
    friend class App;
    
public:
    explicit Window(int w = 0, int h = 0, bool useFullScreen = false);
    ~Window();
    
    bool Init(int w = 0, int h = 0, bool useFullScreen = false);
    
    void Destroy();
    
    const std::string& title() const;
    void title(const std::string& title);
    
    int width() const { return mWidth; }
    int height() const {return mHeight; }
    

    Renderer & renderer() { return *pRenderer; }
    
private:
    int mWidth;
    int mHeight;
    
    std::string mWindowTitle;
    std::shared_ptr<SDL_Window> pSDLWindow;
    
    std::shared_ptr<Renderer> pRenderer;
    
    Window(const Window &);
    
    void operator=(const Window&);
};




#endif /* Window_hpp */
