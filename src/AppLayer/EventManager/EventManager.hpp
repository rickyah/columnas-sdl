//
//  EventManager.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 23/5/16.
//
//

#ifndef EventManager_hpp
#define EventManager_hpp

#include <ctime>
#include <memory>
#include <functional>
#include <map>
#include <list>

#include "EventManagerTypes.hpp"
#include "EventListenerDelegate.hpp"
#include "IEventData.hpp"

/**
 * Holds a list of delegates that listen for an event to be triggered
 */
class EventManager
{
    typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    
public:
    explicit EventManager() {};
    
    bool AddListener(const EventType& type, const EventListenerDelegate &eventDelegate);
    
    bool RemoveListener(const EventType& type, const EventListenerDelegate &eventDelegate);
    
    bool Raise(const std::shared_ptr<IEventData>& pEvent);

private:
    // Holds mappings between an EventType and an a list of EventListenerDelegates
    EventListenerMap _eventListeners;
};

#endif /* EventManager_hpp */
