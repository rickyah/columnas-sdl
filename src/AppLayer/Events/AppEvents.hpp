//
//  AppEvents.hpp
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#ifndef AppEvent_hpp
#define AppEvent_hpp

#include "IEventData.hpp"

class AppQuit_Event : public IEventData
{
public:
    static const EventType sEventType;
    AppQuit_Event():IEventData() {};
    virtual const EventType& type() const { return sEventType; }
};


class AppTouch_Event : public IEventData
{
public:
    
    enum ETouchType { Up, Down, Motion };
    
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
        ETouchType type;
        TouchPosition pos;
        TouchMotion motion;
    };
    
    typedef TouchEventArgs event_args;
    
    AppTouch_Event(AppTouch_Event::event_args args);
    
    TouchEventArgs args() const { return mTouchEventArgs; }
    
    static const EventType sEventType;
    virtual const EventType& type() const { return sEventType; }
    
protected:
    TouchEventArgs mTouchEventArgs;
    
};
#endif /* AppEvents_hpp */
