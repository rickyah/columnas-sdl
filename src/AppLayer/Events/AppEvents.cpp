//
//  AppQuit_Event.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#include "AppEvents.hpp"

// Sent app quit event
const EventType AppQuit_Event::sEventType(0x398625bc);

const EventType AppTouch_Event::sEventType(0x17df1d3f);


AppTouch_Event::AppTouch_Event(TouchEventArgs args):
    IEventData(), mTouchEventArgs(args)
{
    
};


