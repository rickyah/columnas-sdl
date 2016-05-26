//
//  EventQueue.cpp
//
//  Created by Ricardo Amores Hernández on 23/5/16.
//
//
#include "EventQueue.hpp"


bool EventQueue::AddListener(const EventType& type, const EventListenerDelegate &eventDelegate)
{
    EventListenerList& list = _eventListeners[type];
    
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if(eventDelegate.id == (*it).id)
        {
            return false;
        }
   }

    list.push_back(eventDelegate);
    
    return true;
}

bool EventQueue::RemoveListener(const EventType& type, const EventListenerDelegate &eventDelegate)
{
    EventListenerList& list = _eventListeners[type];
    
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (eventDelegate.id == (*it).id)
        {
            list.erase(it);
            return true;
        }
    }

    return false;
}
    
bool EventQueue::Raise(const std::shared_ptr<IEventData>& pEvent)
{
    EventListenerList& list = _eventListeners[pEvent->type()];
    
    for(EventListenerDelegate &dlg : list)
    {
        dlg(pEvent);
    }
    
}

