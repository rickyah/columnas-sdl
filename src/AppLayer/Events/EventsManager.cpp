//
//  EventsManager.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#include "EventsManager.hpp"
EventsManager::EventsManager(std::shared_ptr<Graphics> graphics, std::shared_ptr<EventQueue> eventQueue)
    :pEventQueue(eventQueue), pGraphics(graphics)
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
            case SDL_FINGERUP:
            case SDL_FINGERMOTION:
                pEventQueue->Raise(CreateTouchEvent(event));
                return true;
            case SDL_QUIT:
                pEventQueue->Raise(std::static_pointer_cast<IEventData>(std::make_shared<AppQuit_Event>()));
                return false;
        }
    }
    
    return true;
}

std::shared_ptr<IEventData> EventsManager::CreateTouchEvent(const SDL_Event& event)
{
    static AppTouch_Event::event_args args;
    
    switch(event.type)
    {
        case SDL_FINGERDOWN:
            args.type = AppTouch_Event::ETouchType::Down;
            break;
            
        case SDL_FINGERUP:
            args.type = AppTouch_Event::ETouchType::Up;
            break;
            
        case SDL_FINGERMOTION:
            args.type = AppTouch_Event::ETouchType::Motion;
            args.motion = {event.tfinger.dx, event.tfinger.dy};
            break;
    }
    
    args.pos = {
        static_cast<int>(pGraphics->width() * event.tfinger.x),
        static_cast<int>(pGraphics->height() * event.tfinger.y),
        event.tfinger.x,
        event.tfinger.y
    };
    
    return std::static_pointer_cast<IEventData>(std::make_shared<AppTouch_Event>(args));
}


