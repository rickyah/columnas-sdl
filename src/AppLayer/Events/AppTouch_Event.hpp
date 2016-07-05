//
//  AppTouch_Event.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 18/6/16.
//
//

#ifndef AppTouch_Event_h
#define AppTouch_Event_h

#include "DataTypes.hpp"
#include "InputEventDataTypes.hpp"

class AppTouch_Event : public IEventData
{
public:


    
    struct TouchPosition
    {
        // Coordinates in the default screen
        int x, y;

        // Normalized in range [0,1]
        double nx, ny;
    };

    struct TouchMotion
    {
        float dx, dy;
    };

    struct TouchEventArgs
    {
        PointerInputEventType type;
        TouchPosition pos;
        TouchMotion motion;
    };
    

    AppTouch_Event(TouchEventArgs args):IEventData(), mArgs(args) {}

    const TouchEventArgs& args() const { return mArgs; }

    typedef TouchEventArgs event_args_type;
    
    static const EventType sEventType;
    virtual const EventType& type() const { return sEventType; }
    
        ~AppTouch_Event(){}
protected:
    TouchEventArgs mArgs;
    
};


#endif /* AppTouch_Event_h */
