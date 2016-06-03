//
//  FSM.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 1/6/16.
//
//

#ifndef FSM_hpp
#define FSM_hpp

#include <unordered_map>
#include <memory>


/*
 * Interface defining all the operations required for any state in this FSM
 */
class IState
{
public:
    virtual ~IState() {}
    
    virtual void OnUpdate(double dt) {}
    
    virtual void OnInit() {}
    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void OnCleanup() {}
    
protected:
    IState() {}
    IState(const IState &) {}
private:
    IState& operator=(const IState &);
};



/*
 * Implements a simple finite state machine for game logic. The states used by the machine must be registered
 * in advance and are tracked with a hashable type used as id. This machine does not use the concept of transitions
 * so you can change to any registered state.
 *
 * The TState type must support a set of operations to notify the multiple changes
 *
 *  Note that the actual state change is only performed after a call to the Update method
 *
 *
 *  Template parameters
 *  TStateId: is any type that can act as a key for a defaul unordered_map, as is the data structure used
 *            to keep track of the states. If needed this could be augmented with another template type
 *            that allows defining a new hasher (see map::hasher)
 *  TState:  The type for the states. Normally this would be a base class for all the states. Note that the 
 *           states using this FSM must expose a minimun set of operations (methods) for this to work
 *           Take a look at the IState interface above for an example implementation, or to use it as a base
 *           class for implementors.
 */

template <typename TStateId, typename TState>
class FSM
{
    using TStatePtr = std::shared_ptr<IState>;
    
public:

    
    TStatePtr currentState() const { return pCurrentState; }
    
    /*
     *  Changes to a new registered state what was previously registerd in the FSM
     *  The actual state change won't be made until the next call to update.
     *
     *  @return false if the state was not registered in the FSM, otherwise returns true.
     */
    bool ChangeTo(TStateId id);
    
    /*
     * Register an state to be used by the FSM
     *
     * @return false if an state with the same TStateID was already registered in the FSM, otherwise returns true.
     */
    bool RegisterState(TStateId id, TStatePtr pState);
    
    /*
     * Removes an state to be used by the FSM
     *
     * @return false if there are no state with the same TStateID registered in the FSM, otherwise returns true.
     */
    
    bool RemoveState(TStateId id);
    
    /*
     * Updates the state machine and performs the state changes
     */
    void Update(double dt);
    
    
    ~FSM();
    
private:
    std::unordered_map<TStateId, TStatePtr> mStates;
    
    TStatePtr pNextState;
    TStatePtr pCurrentState;
    
};


////////////////////////////////////////////////////////////////////////
// Implementation

template <typename TStateId, typename TState>
bool FSM<TStateId, TState>::RegisterState(TStateId id, TStatePtr pState)
{
    pState->OnInit();
    
    auto pair = std::make_pair(id, pState);
    
    return mStates.insert(pair).second;
}

template <typename TStateId, typename TState>
bool FSM<TStateId, TState>::ChangeTo(TStateId id)
{
    auto nextState = mStates[id];
    
    if (nextState == nullptr) return false;
    
    if (_currentState != nextState)
    {
        pNextState = nextState;
    }
    
    return true;
}

template <typename TStateId, typename TState>
bool FSM<TStateId, TState>::RemoveState(TStateId id)
{
    auto pState = mStates[id];
    
    if (pState == nullptr) return false;
    
    if (pNextState == pState)
    {
        pNextState = nullptr;
    }
    
    if (pCurrentState == pState)
    {
        pState->OnExit();
        pCurrentState = nullptr;
    }
    
    pState->OnCleanup();

    mStates.erase(id);
    
    return true;
}

template <typename TStateId, typename TState>
void FSM<TStateId, TState>::Update(double dt)
{
    if(pNextState)
    {
        if(pCurrentState)
        {
            pCurrentState->OnExit();
        }
        
        std::swap(pCurrentState, pNextState);
        
        pCurrentState->OnEnter();
        
        pNextState = nullptr;
    }
    
    if(pCurrentState)
    {
        pCurrentState->OnUpdate(dt);
    }
}

template <typename TStateId, typename TState>
FSM<TStateId, TState>::~FSM()
{
    mStates.clear();
}

#endif /* FSM_hpp */
