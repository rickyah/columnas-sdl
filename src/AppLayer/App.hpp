//
//  CApp.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef App_hpp
#define App_hpp

#include <memory>
#include "SDL.h"
#include "Graphics.hpp"
#include "EventManager.hpp"

class App
{
public:
    App();
    ~App();
    
    std::shared_ptr<Graphics> graphics() { return _pGraphics; }
    std::shared_ptr<EventManager> eventManager() { return _pEventManager; }
   
private:
    std::shared_ptr<Graphics> _pGraphics;
    std::shared_ptr<EventManager> _pEventManager;
    
};

#endif /* App_hpp */
