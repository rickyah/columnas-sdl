//
//  AppMouse_Event.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 18/6/16.
//
//

#ifndef AppMouse_Event_h
#define AppMouse_Event_h

#include "DataTypes.hpp"
#include "InputEventDataTypes.hpp"

class AppMouse_Event : public IEventData
{
public:
   
    struct MouseMotion
    {
        float dx, dy;
    };
    
    struct MouseEventArgs
    {
        PointerInputEventType type;
        Position pos;
        MouseMotion motion;
    };
    
    AppMouse_Event(MouseEventArgs args):IEventData(), mArgs(args) {}
    const MouseEventArgs& args() const { return mArgs; }
    
    typedef MouseEventArgs event_args_type;
    
    static const EventType sEventType;
    virtual const EventType& type() const { return sEventType; }
    
    ~AppMouse_Event(){}
    
protected:
    MouseEventArgs mArgs;
    
};
#endif /* AppMouse_Event_h */
