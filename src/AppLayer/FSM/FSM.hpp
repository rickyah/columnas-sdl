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
 * Use this hasher function if you want to used the FSM using enum as keys
 *
 * FSM<MyEnum, IState, EnumHasher> myFSM;
 */
struct EnumHasher
{
    template <typename T>
    inline
    typename std::enable_if<std::is_enum<T>::value, std::size_t>::type
    operator ()(T const value) const
    {
        return static_cast<std::size_t>(value);
    }
};


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
 *  THash:   Hash function to use, defaults to the default std::hash function defined for the TStateId type
 */




template <typename TStateId, typename TState, typename THash = std::hash<TStateId>>
class FSM
{
    
public:
    using TStatePtr = std::shared_ptr<TState>;
    
    struct StateInfo
    {
        TStateId id = TStateId();
        TStatePtr ptr = nullptr;
    };
    
    StateInfo currentState() const { return mCurrentStateInfo; }
    
    StateInfo nextState() const { return mNextStateInfo; }
    
    TStatePtr FindState(TStateId id);
    
    /*
     * Changes to a new registered state what was previously registerd in the FSM
     * The actual state change won't be made until the next call to update.
     *
     * Is marked as const because the internal state of the FSM does not really change, and this will allow states
     * to get a const reference to the state machine and change to another state, but without accessing the rest
     * of the non-const API.
     *
     * @return false if the state was not registered in the FSM, otherwise returns true.
     */
    bool ChangeTo(TStateId id) const;
    
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
    std::unordered_map<TStateId, TStatePtr, THash> mStates;
    
    // Made mutable to make the ChangeTo method const, as changing this element does not
    // really change the behaviour of the class until a call to OnUpdate is made
    mutable StateInfo mNextStateInfo;
    StateInfo mCurrentStateInfo;
    
};


////////////////////////////////////////////////////////////////////////
// Implementation

template <typename TStateId, typename TState, typename THash>
bool FSM<TStateId, TState, THash>::RegisterState(TStateId id, TStatePtr pState)
{
    pState->OnInit();
    
    auto pair = std::make_pair(id, pState);
    
    return mStates.insert(pair).second;
}

template <typename TStateId, typename TState, typename THash>
bool FSM<TStateId, TState, THash>::ChangeTo(TStateId id) const
{
    auto nextState = mStates.find(id);
    
    if (nextState == mStates.end()) return false;
    
    if (mCurrentStateInfo.ptr != nextState->second)
    {
        mNextStateInfo.id = id;
        mNextStateInfo.ptr = nextState->second;
    }
    
    return true;
}

template <typename TStateId, typename TState, typename THash>
bool FSM<TStateId, TState, THash>::RemoveState(TStateId id)
{
    auto pState = mStates[id];
    
    if (pState == nullptr) return false;
    
    if (mNextStateInfo.ptr == pState)
    {
        mNextStateInfo.id = TStateId();
        mNextStateInfo.ptr = nullptr;
    }
    
    if (mCurrentStateInfo.ptr == pState)
    {
        pState->OnExit();
        
        
        mCurrentStateInfo.id = TStateId();
        mCurrentStateInfo.ptr = nullptr;
    }
    
    pState->OnCleanup();

    mStates.erase(id);
    
    return true;
}

template <typename TStateId, typename TState, typename THash>
void FSM<TStateId, TState, THash>::Update(double dt)
{
    if(mNextStateInfo.ptr)
    {
        if(mCurrentStateInfo.ptr)
        {
            mCurrentStateInfo.ptr->OnExit();
        }
        
        std::swap(mCurrentStateInfo, mNextStateInfo);
        
        mCurrentStateInfo.ptr->OnEnter();
        
        mNextStateInfo.id = TStateId();
        mNextStateInfo.ptr = nullptr;
    }
    
    if(mCurrentStateInfo.ptr)
    {
        mCurrentStateInfo.ptr->OnUpdate(dt);
    }
}

template <typename TStateId, typename TState, typename THash>
typename FSM<TStateId, TState, THash>::TStatePtr FSM<TStateId, TState, THash>::FindState(TStateId id)
{
    auto state = mStates.find(id);
    
    if (state == mStates.end()) return nullptr;
    
    return state->second;
}

template <typename TStateId, typename TState, typename THash>
FSM<TStateId, TState, THash>::~FSM()
{
    mStates.clear();
}

#endif /* FSM_hpp */
