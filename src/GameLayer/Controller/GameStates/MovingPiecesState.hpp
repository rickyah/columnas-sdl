//
//  MovingPiecesState.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#ifndef MovingPiecesState_h
#define MovingPiecesState_h


#include "ColumnsGameFSMDefinitions.hpp"
#include "ColumnsGameController.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

// Need this FD to typedef the FSM available when the player is in the MovingPiecesState 
class InputState;

/*
 * The player will able to control the block while this state is active
 */
class MovingPiecesState : public BaseColumnsGameState
{
    
public:
    enum MovementSubStateIds
    {
        Waiting,
        Received_Input,
        Moving_Block,
        Droping_Block
    };
    
    typedef FSM<MovementSubStateIds, InputState, EnumHasher> MovingSubstatesFSM;
    
    MovingPiecesState(const ColumnsGameFSM &fsm,
                      ColumnsGameController &controller,
                      EventQueue & eventQueue);
    
    
    virtual void OnInit() override;
    virtual void OnCleanup() override;
    virtual void OnUpdate(double dt) override;
    
private:
    MovingSubstatesFSM mSubFSM;
    
    void OnTouchEvent(std::shared_ptr<IEventData> event);
    
    bool HasMotion(AppTouch_Event::TouchMotion motion) const;
    EventListenerDelegate mOnTouchEventDlg;
    EventQueue &mEventQueueRef;
    
};



//////////////////////////////////////////////////
// Input sub-FSM States definition

/*
 * InputState childs are used in the sub FSM available when the player is moving the block
 */

class InputState : public IState
{
public:
    InputState(MovingPiecesState::MovingSubstatesFSM &fsm, EventQueue &eventQueue):
        mFSM(fsm),
        mEventQueue(eventQueue)
    {}
    
    virtual ~InputState() {};
    
    // Template Method Pattern: called by the TouchEventListener and allows
    // derived classes to respond to input, and saves each InputState to register/unregister
    // a listener to AppTouch_Event each time we change states
    virtual void OnTouchEvent(std::shared_ptr<AppTouch_Event> event) = 0;
protected:
    
    AppTouch_Event::TouchMotion FilterMotion(const AppTouch_Event::TouchMotion motion, float minValue) const;
    
    MovingPiecesState::MovingSubstatesFSM &mFSM;
    EventQueue &mEventQueue;
};


/*
 * Player has not issued any input yet
 */
class WaitingInputState : public InputState
{
public:
    WaitingInputState(MovingPiecesState::MovingSubstatesFSM &fsm, EventQueue &eventQueue)
        :InputState(fsm, eventQueue)
    {}
    
    void OnTouchEvent(std::shared_ptr<AppTouch_Event> event) override;
};

/*
 * Player has touched the screen. Depending on the type of movement we need to choose between moving
 * the block or permuting the pieces in the block
 * 
 * Enters from states:
 * WaitingInputState
 * 
 * Changes to states:
 * MovingBlockPieceInputState   --> AppTouch_Event::TouchType::Motion if player issues a motion in a given timeframe
 * DroppingBlockPieceInputState --> no motion events in a given timeframe
 * WaitingInputState            --> AppTouch_Event::TouchType::Up if given before the timeframe.
 *                                  This permutes the player's block
 */
class ReceivedInputState : public InputState
{
public:
    ReceivedInputState(MovingPiecesState::MovingSubstatesFSM &fsm, EventQueue &eventQueue, ColumnsGameController &controller)
        :InputState(fsm, eventQueue), mControllerRef(controller)
    {}
    
   
    
    void OnEnter() override;
    void OnExit() override;
    void OnUpdate(double dt) override;
    
    void OnTouchEvent(std::shared_ptr<AppTouch_Event> event) override;
    
private:
    ColumnsGameController& mControllerRef;
    int mCurrentElapsedMs;
};


/*
 * Player is moving the finger across the screen to move the block.
 *
 * Enters from states:
 * WaitingInputState
 *
 * Changes to states:
 * WaitingInputState --> AppTouch_Event::TouchType::Up
 */
class MovingBlockPieceInputState : public InputState
{
public:
    
    MovingBlockPieceInputState(MovingPiecesState::MovingSubstatesFSM &fsm,
                               EventQueue &eventQueue,
                               ColumnsGameController &controller)
        :InputState(fsm, eventQueue), mControllerRef(controller)
    {}
    
    double motionAccToMoveX = 0.13;
    double motionAccToMoveY = 0.08;
    void OnEnter() override;
    void OnUpdate(double dt) override;
    void OnTouchEvent(std::shared_ptr<AppTouch_Event> event) override;
    
    AppTouch_Event::TouchMotion initialMotion;
    
private:
    void AccumulateMotion(const AppTouch_Event::TouchMotion &src);

    // positive to the right, negative to the left. 0 for no motion
    float mMotionAccumulator;
    
    ColumnsGameController &mControllerRef;
};

class DroppingBlockPieceInputState : public InputState
{
public:
    
    DroppingBlockPieceInputState(MovingPiecesState::MovingSubstatesFSM &fsm,
                                 EventQueue &eventQueue,
                                 ColumnsGameController &controller)
    :InputState(fsm, eventQueue), mControllerRef(controller)
    {}
    

    void OnUpdate(double dt) override;
    void OnTouchEvent(std::shared_ptr<AppTouch_Event> event) override;
    
    
private:    
    ColumnsGameController &mControllerRef;
};
#endif /* MovingPiecesState_h */
