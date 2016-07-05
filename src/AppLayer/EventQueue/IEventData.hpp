//
//  IEventData.hpp
//
//
//  Created by Ricardo Amores Hernández on 23/5/16.
//
//

#ifndef IEventData_hpp
#define IEventData_hpp

#include <ctime>
#include <cstdint>

#include "EventQueueTypes.hpp"

/**
 * This is the base class for all data sent when an event is triggered
 * Each derived class must define a member
 * static const EventType sEventType
 * which defines the event's Type, and override the type() method to
 * return that member's value
 */
class IEventData
{
    
public:
    IEventData(): mTimeStamp(std::time(nullptr)){}
    
    virtual ~IEventData() {}
    uint64_t timeStamp() const { return mTimeStamp; }
    
    virtual const EventType& type() const = 0;
    
private:
    
    const uint64_t mTimeStamp;
};



#endif /* IEventData_hpp */
