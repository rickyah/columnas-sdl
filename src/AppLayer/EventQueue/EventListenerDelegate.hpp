//
//  EventListenerDelegate.hpp
//
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef EventListenerDelegate_hpp
#define EventListenerDelegate_hpp

#include <memory>
#include <functional>

#include "IEventData.hpp"

typedef std::function<void (std::shared_ptr<IEventData>)> EventListenerFunc;

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
    
    EventListenerDelegate(EventListenerFunc func):mFunction(func), id(sNextId++) { }
    
    void operator() (std::shared_ptr<IEventData> pEventData)
    {
        mFunction(pEventData);
    }
    
private:
    
    //auto-incremental id
    static AutoIncrementalIdType sNextId;
    
    // target
    std::function<void (std::shared_ptr<IEventData>)> mFunction;
};


#endif /* EventListenerDelegate_hpp */
