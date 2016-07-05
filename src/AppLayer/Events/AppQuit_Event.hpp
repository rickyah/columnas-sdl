//
//  AppQuit_Event.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 18/6/16.
//
//

#ifndef AppQuit_Event_h
#define AppQuit_Event_h


class AppQuit_Event : public IEventData
{
public:
    static const EventType sEventType;
    AppQuit_Event():IEventData() {};
    virtual const EventType& type() const { return sEventType; }
};


#endif /* AppQuit_Event_h */
