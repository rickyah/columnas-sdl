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
#include "EventQueue.hpp"

class App
{
public:
    App();
    ~App();
    
    std::shared_ptr<Graphics> graphics() { return _pGraphics; }
    std::shared_ptr<EventQueue> eventManager() { return _pEventQueue; }
   
private:
    std::shared_ptr<Graphics> _pGraphics;
    std::shared_ptr<EventQueue> _pEventQueue;
    
};

#endif /* App_hpp */
