//
//  EventQueue.cpp
//
//  Created by Ricardo Amores Hernández on 23/5/16.
//
//
#include "EventQueue.hpp"

EventQueue::~EventQueue()
{
    RemoveAllListeners();
}


bool EventQueue::AddListener(const EventType& type, const EventListenerDelegate &eventDelegate)
{
    EventListenerList& list = mEventListeners[type];
    
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


EventListenerDelegate EventQueue::AddListener(const EventType& type, const std::function<void (std::shared_ptr<IEventData>)> &func)
{
    EventListenerDelegate dlg(func);
    
    this->AddListener(type, dlg);
    
    return dlg;
}

bool EventQueue::RemoveListener(const EventType& type, EventListenerDelegate::AutoIncrementalIdType eventId)
{
    EventListenerList& list = mEventListeners[type];
    
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (eventId == it->id)
        {
            list.erase(it);
            return true;
        }
    }
    
    return false;
}

bool EventQueue::RemoveListener(const EventType& type, const EventListenerDelegate &eventDelegate)
{
    return RemoveListener(type, eventDelegate.id);
}
    
void EventQueue::Raise(std::shared_ptr<IEventData> pEvent)
{
    EventListenerList& list = mEventListeners[pEvent->type()];
    
    for(EventListenerDelegate &dlg : list)
    {
        dlg(pEvent);
    }
    
}

void EventQueue::RemoveAllListeners()
{
    mEventListeners.clear();
}
