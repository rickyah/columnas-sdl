//
//  EventsManager.hpp
//
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#ifndef EventsManager_hpp
#define EventsManager_hpp

#include <memory>

#include <SDL2/SDL.h>
#include "Window.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

class EventsManager
{
public:
    EventsManager(std::shared_ptr<Window> graphics,
                  std::shared_ptr<EventQueue> eventQueue);
    
    bool Update();
    
private:
    std::shared_ptr<Window> pWindow;
    std::shared_ptr<EventQueue> pEventQueue;
    
    std::shared_ptr<IEventData> CreateMouseEvent(const SDL_Event& event);
    std::shared_ptr<IEventData> CreateTouchEvent(const SDL_Event& event);
    std::shared_ptr<IEventData> CreateKeyboardEvent(const SDL_Event& event);
};
#endif /* EventsManager_hpp */
