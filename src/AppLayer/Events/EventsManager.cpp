//
//  EventsManager.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#include "EventsManager.hpp"
EventsManager::EventsManager(std::shared_ptr<EventQueue> eventQueue):pEventQueue(eventQueue)
{
    SDL_InitSubSystem(SDL_INIT_EVENTS);
}


bool EventsManager::Update()
{
    static SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_FINGERDOWN:
                //pEventQueue->Raise(std::static_pointer_cast<IEventData>(std::make_shared<AppTouch_Event>()));
                break;
    
            case SDL_FINGERUP:
                pEventQueue->Raise(std::static_pointer_cast<IEventData>(std::make_shared<AppTouch_Event>()));
                break;
        
            case SDL_FINGERMOTION:
               // pEventQueue->Raise(std::static_pointer_cast<IEventData>(std::make_shared<AppTouch_Event>()));
                break;

            case SDL_QUIT:
                pEventQueue->Raise(std::static_pointer_cast<IEventData>(std::make_shared<AppQuit_Event>()));
                return false;
                break;
                
            default:
                SDL_Log("Polled unknown event %d", event.type);
        }    
    }
    
    return true;
}


