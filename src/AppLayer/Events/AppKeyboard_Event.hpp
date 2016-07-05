//
//  AppKeybord_Event.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 18/6/16.
//
//

#ifndef AppKeybord_Event_h
#define AppKeybord_Event_h

#include <SDL2/SDL_events.h>

class AppKeyboard_Event : public IEventData
{
public:
    enum class KeyboardInputType
    {
        KeyDown,
        KeyUp
    };
    struct KeyboardEventArgs
    {
        KeyboardInputType type;
        SDL_Scancode scancode;
    };
    
    AppKeyboard_Event(KeyboardEventArgs args):IEventData(), mArgs(args) {}
    
    const KeyboardEventArgs& args() const { return mArgs; }
    
    typedef KeyboardEventArgs event_args_type;
    
    static const EventType sEventType;
    virtual const EventType& type() const { return sEventType; }
    
    virtual ~AppKeyboard_Event() {}
    
protected:
    KeyboardEventArgs mArgs;
};

#endif /* AppKeybord_Event_h */
