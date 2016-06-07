//
//  ColumnsGameStates.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#ifndef ColumnsGameStates_hpp
#define ColumnsGameStates_hpp

#include "FSM.hpp"
#include "ColumnsGameFSMDefinitions.hpp"
#include "ColumnsGameController.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

/*
 * Base class for all states in the Columns game
 */
class ColumnsGameController;

class ColumnsGameState : public IState
{
protected:
    ColumnsGameState(const ColumnsGameFSM &fsm, ColumnsGameController &controller):
    mFSM(fsm),
    mControllerRef(controller)
    
    {}
    
    const ColumnsGameFSM &mFSM;
    ColumnsGameController &mControllerRef;
    
private:
    ColumnsGameState(const ColumnsGameState &);
    ColumnsGameState & operator=(const ColumnsGameState &);
};


/*
 * The player will able to control the block while this state is active
 */
class MovingPiecesState : public ColumnsGameState
{
public:
    MovingPiecesState(const ColumnsGameFSM &fsm,
                      ColumnsGameController &controller,
                      EventQueue & eventQueue);
    
    
    virtual void OnInit() override;
    virtual void OnCleanup() override;
    virtual void OnUpdate(double dt) override;
    
private:
    
    void OnTouchEvent(std::shared_ptr<IEventData> event);
    AppTouch_Event::TouchMotion FilterMotion(AppTouch_Event::TouchMotion motion) const;
    EventListenerDelegate mOnTouchEventDlg;
    EventQueue &mEventQueueRef;
    
};

/*
 * Search and destroy the pieces that can be removed after a block has been placed
 *
 */

class RemovingPiecesState : public ColumnsGameState
{
public:
    RemovingPiecesState(const ColumnsGameFSM &fsm,
                        ColumnsGameController &controller)
    :ColumnsGameState(fsm, controller)
    {}
};

/*
 *
 */

class DroppingPiecesState : public ColumnsGameState
{
public:
    DroppingPiecesState(const ColumnsGameFSM &fsm,
                        ColumnsGameController &controller)
    :ColumnsGameState(fsm, controller)
    {}
    
    DroppingPiecesState(const DroppingPiecesState &) = delete;
    DroppingPiecesState & operator=(const DroppingPiecesState &) = delete;
};


#endif /* ColumnsGameStates_hpp */
