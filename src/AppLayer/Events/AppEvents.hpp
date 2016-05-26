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
    static const EventType sEventType;
    AppTouch_Event():IEventData() {};
    virtual const EventType& type() const { return sEventType; }
};
#endif /* AppEvents_hpp */
