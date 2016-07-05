//
//  EventsManager.cpp
//
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#include "EventsManager.hpp"
EventsManager::EventsManager(std::shared_ptr<Window> ptrWindow, std::shared_ptr<EventQueue> eventQueue)
    :pEventQueue(eventQueue), pWindow(ptrWindow)
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
                
            case SDL_KEYDOWN:
                pEventQueue->Raise(CreateKeyboardEvent(event));
                return true;
                
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
                pEventQueue->Raise(CreateMouseEvent(event));
                return true;
                
            case SDL_QUIT:
                pEventQueue->Raise(std::static_pointer_cast<IEventData>(std::make_shared<AppQuit_Event>()));
                return false;
        }
    }
    
    return true;
}

std::shared_ptr<IEventData> EventsManager::CreateMouseEvent(const SDL_Event& event)
{
    static AppMouse_Event::event_args_type args;
    
    switch(event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            args.type = PointerInputEventType::Down;
            args.pos.x = event.button.x;
            args.pos.y = event.button.y;
            break;
            
        case SDL_MOUSEBUTTONUP:
            args.type = PointerInputEventType::Up;
            args.pos.x = event.button.x;
            args.pos.y = event.button.y;
            break;
            
        case SDL_MOUSEMOTION:
            args.type = PointerInputEventType::Motion;
            args.pos.x = event.button.x;
            args.pos.y = event.button.y;
            break;
    }
    
    return std::static_pointer_cast<IEventData>(std::make_shared<AppMouse_Event>(args));
}


std::shared_ptr<IEventData> EventsManager::CreateTouchEvent(const SDL_Event& event)
{
    static AppTouch_Event::event_args_type args;
    
    switch(event.type)
    {
        case SDL_FINGERDOWN:
            args.type = PointerInputEventType::Down;
            break;
            
        case SDL_FINGERUP:
            args.type = PointerInputEventType::Up;
            break;
            
        case SDL_FINGERMOTION:
            args.type = PointerInputEventType::Motion;
            args.motion = {event.tfinger.dx, event.tfinger.dy};
            break;
    }
    
    args.pos = {
        static_cast<int>(pWindow->width() * event.tfinger.x),
        static_cast<int>(pWindow->height() * event.tfinger.y),
        event.tfinger.x,
        event.tfinger.y
    };
    
    return std::static_pointer_cast<IEventData>(std::make_shared<AppTouch_Event>(args));
}

std::shared_ptr<IEventData> EventsManager::CreateKeyboardEvent(const SDL_Event& event)
{
    static AppKeyboard_Event::event_args_type args;
    
    switch(event.type)
    {
        case SDL_KEYDOWN:
            args.type = AppKeyboard_Event::KeyboardInputType::KeyDown;
            break;
            
        case SDL_KEYUP:
            args.type = AppKeyboard_Event::KeyboardInputType::KeyUp;
            break;
    }
    
    args.scancode = event.key.keysym.scancode;
    
    return std::static_pointer_cast<IEventData>(std::make_shared<AppKeyboard_Event>(args));
}
