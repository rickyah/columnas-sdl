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

/*
 * The player will able to control the block while this state is active
 */
class MovingPiecesState : public BaseColumnsGameState
{
    
    enum MovementState
    {
        Waiting,
        Accept_Input,
        Moving_Left,
        Moving_Right,
        Moving_Down,
        Permuting
    };
    
public:
    MovingPiecesState(const ColumnsGameFSM &fsm,
                      ColumnsGameController &controller,
                      EventQueue & eventQueue);
    
    
    virtual void OnInit() override;
    virtual void OnCleanup() override;
    virtual void OnUpdate(double dt) override;
    
private:
    MovementState mMovementState = MovementState::Waiting;
    
    void OnTouchEvent(std::shared_ptr<IEventData> event);
    
    bool HasMotion(AppTouch_Event::TouchMotion motion) const;
    AppTouch_Event::TouchMotion FilterMotion(AppTouch_Event::TouchMotion motion) const;
    EventListenerDelegate mOnTouchEventDlg;
    EventQueue &mEventQueueRef;
    
};





#endif /* MovingPiecesState_h */
