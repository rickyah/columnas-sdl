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


/**
 * An event will be defined by an unique number (GUID)
 * This approach is taken from Game Coding Complete book
 */
typedef unsigned long EventType;


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
    IEventData() : _timeStamp(std::time(nullptr)){}
    
    virtual ~IEventData() {}
    uint64_t timeStamp() const { return _timeStamp; }
    
    //
    virtual const EventType& type() const = 0;

private:
    
    const uint64_t _timeStamp;
};


/**
 * Functor defining the target function/method to be called in a listener for an event
 * Wraps the std::function with an unique Id that will allow to unregister the listener
 * as we can't compare std::function instances
 * TODO: consider using FastDelegates for c++ 
 * http://codereview.stackexchange.com/questions/14730/impossibly-fast-delegate-in-c11
 */
class EventListenerDelegate
{
    
public:
    // unique id for an event listener
    typedef uint_fast64_t AutoIncrementalIdType;
    const AutoIncrementalIdType id;
    
    EventListenerDelegate(std::function<void (std::shared_ptr<IEventData>)> func):_function(func), id(_nextId++) { }
    
    void operator() (std::shared_ptr<IEventData> pEventData)
    {
        _function(pEventData);
    }
    
private:
    
    //auto-incremental id
    static AutoIncrementalIdType _nextId;
    
    // target
    std::function<void (std::shared_ptr<IEventData>)> _function;
};


/**
 * Holds a list of delegates that listen for an event to be triggered
 */
class EventManager
{
    typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    
public:
    explicit EventManager();
    
    bool AddListener(const EventType& type, const EventListenerDelegate &eventDelegate);
    
    bool RemoveListener(const EventType& type, const EventListenerDelegate &eventDelegate);
    
    bool Raise(const std::shared_ptr<IEventData>& pEvent);

private:
    // Holds mappings between an EventType and an a list of EventListenerDelegates
    EventListenerMap _eventListeners;
};

#endif /* EventManager_hpp */
