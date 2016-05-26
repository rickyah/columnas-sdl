//
//  EventListenerDelegate.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef EventListenerDelegate_hpp
#define EventListenerDelegate_hpp

#include <memory>
#include <functional>

#include "IEventData.hpp"

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


#endif /* EventListenerDelegate_hpp */
