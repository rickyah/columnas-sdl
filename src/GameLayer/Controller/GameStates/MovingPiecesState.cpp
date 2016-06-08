//
//  MovingBlockState.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#include "MovingPiecesState.hpp"
#include <cmath>

MovingPiecesState::MovingPiecesState(const ColumnsGameFSM &fsm,
                                     ColumnsGameController &controller,
                                     EventQueue & eventQueue):
    BaseColumnsGameState(fsm, controller)
    ,mEventQueueRef(eventQueue)
    ,mOnTouchEventDlg(EventListenerDelegate(std::bind(&MovingPiecesState::OnTouchEvent, this, std::placeholders::_1)))
{
    mSubFSM.RegisterState(MovingPiecesState::MovementSubStateIds::Waiting,
                          std::make_shared<WaitingInputState>(mSubFSM,
                                                              mEventQueueRef));
    
    mSubFSM.RegisterState(MovingPiecesState::MovementSubStateIds::Received_Input,
                          std::make_shared<ReceivedInputState>(mSubFSM,
                                                               mEventQueueRef,
                                                               controller));
    
    mSubFSM.RegisterState(MovingPiecesState::MovementSubStateIds::Moving_Block,
                          std::make_shared<MovingBlockPieceInputState>(mSubFSM,
                                                                       mEventQueueRef,
                                                                       controller));
    
    mSubFSM.RegisterState(MovingPiecesState::MovementSubStateIds::Droping_Block,
                          std::make_shared<DroppingBlockPieceInputState>(mSubFSM,
                                                                         mEventQueueRef,
                                                                         controller));
    
    mSubFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Waiting);
}

void MovingPiecesState::OnTouchEvent(std::shared_ptr<IEventData> event)
{
    auto ptr = std::static_pointer_cast<AppTouch_Event>(event);
    
    mSubFSM.currentState().ptr->OnTouchEvent(ptr);
}

void MovingPiecesState::OnUpdate(double dt)
{
    mSubFSM.Update(dt);
    
    static int32_t passedTime = 0;
    passedTime += dt;
    
    bool endGame = false;
    int timePerDropMs = 1000;
    if (passedTime >= timePerDropMs)
    {

        if(!mControllerRef.MoveDown())
        {
            if(! mControllerRef.ResetPlayerBlock())
            {
                
            }
        }

        // update view
        passedTime = std::max(0, passedTime -timePerDropMs);
    }
    
}

void MovingPiecesState::OnInit()
{
    mEventQueueRef.AddListener(AppTouch_Event::sEventType, mOnTouchEventDlg);
}

void MovingPiecesState::OnCleanup()
{
    mEventQueueRef.RemoveListener(AppTouch_Event::sEventType, mOnTouchEventDlg);
}

bool MovingPiecesState::HasMotion(AppTouch_Event::TouchMotion motion) const
{
    if (std::abs(motion.dx) > 0 || std::abs(motion.dy) > 0) return true;
    
    return false;
}


//////////////////////////////////////////////////
// Input sub-FSM States implementation

AppTouch_Event::TouchMotion InputState::FilterMotion(AppTouch_Event::TouchMotion motion, float minValue) const
{
    if (std::abs(motion.dx) < minValue)
    {
        motion.dx = 0;
    }
    
    return motion;
}


void WaitingInputState::OnTouchEvent(std::shared_ptr<AppTouch_Event> event)
{
    if (event->args().type == AppTouch_Event::ETouchType::Down)
    {
        mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Received_Input);
    }
}


void ReceivedInputState::OnEnter()
{
    mCurrentElapsedMs = 0;
}

void ReceivedInputState::OnUpdate(double dt)
{
    mCurrentElapsedMs += dt;
    
    if (mCurrentElapsedMs >= mControllerRef.waitForLongPressMs())
    {
        mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Droping_Block);
    }
}

void ReceivedInputState::OnExit()
{
    if(mFSM.nextState().id == MovingPiecesState::MovementSubStateIds::Moving_Block)
    {
        return;
    }
    
    if (mCurrentElapsedMs < mControllerRef.waitForLongPressMs())
    {
        mControllerRef.PermutePlayerBlockPieces();
    }
}

void ReceivedInputState::OnTouchEvent(std::shared_ptr<AppTouch_Event> event)
{
    if (mCurrentElapsedMs <= mControllerRef.waitForLongPressMs())
    {
        if (event->args().type == AppTouch_Event::ETouchType::Up)
        {
            mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Waiting);
            return;
        }
        
        if (event->args().type == AppTouch_Event::ETouchType::Motion)
        {
            auto filteredMotion = FilterMotion(event->args().motion, mControllerRef.minValueXMotion());
            
            if ( filteredMotion.dx != 0)
            {
                // Save this initial motion
                auto movingState = std::static_pointer_cast<MovingBlockPieceInputState>(mFSM.FindState(MovingPiecesState::MovementSubStateIds::Moving_Block));

                if (movingState)
                {
                    movingState->initialMotion = filteredMotion;
                }
                
                mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Moving_Block);
            }
        }
    }
}


void MovingBlockPieceInputState::OnEnter()
{
    mMotionAccumulator = 0.0;
}

void MovingBlockPieceInputState::OnUpdate(double dt)
{
    // left or right movement accumulates up to a point
    // and then we move the block. Keep the remainder instead of
    // zeroing the accumulation
    if(mMotionAccumulator > motionAccToMoveX)
    {
        mControllerRef.MoveRight();
        mMotionAccumulator -= motionAccToMoveX;
    }
    else if(mMotionAccumulator < -motionAccToMoveX)
    {
        mControllerRef.MoveLeft();
        mMotionAccumulator += motionAccToMoveX;
    }
}

void MovingBlockPieceInputState::OnTouchEvent(std::shared_ptr<AppTouch_Event> event)
{
    if (event->args().type == AppTouch_Event::ETouchType::Up)
    {
        mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Waiting);
        return;
    }
    
    
    if (event->args().type == AppTouch_Event::ETouchType::Motion)
    {
        AccumulateMotion(event->args().motion);
    }
}

void MovingBlockPieceInputState::AccumulateMotion(const AppTouch_Event::TouchMotion &src)
{
    auto filteredMotion = FilterMotion(src, mControllerRef.minValueXMotion());
    
    // If the player changes direction quickly, we just override the
    // accumulated motion with the new value
    if (mMotionAccumulator * src.dx < 0)
    {
         mMotionAccumulator = filteredMotion.dx;
    }
    else
    {
        mMotionAccumulator += filteredMotion.dx;
    }
    
}

void DroppingBlockPieceInputState::OnUpdate(double dt)
{
    // TODO: Limit speed
    if(!mControllerRef.MoveDown())
    {
        mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Waiting);
    }
}

void DroppingBlockPieceInputState::OnTouchEvent(std::shared_ptr<AppTouch_Event> event)
{
    if(event->args().type == AppTouch_Event::ETouchType::Up)
    {
        mFSM.ChangeTo(MovingPiecesState::MovementSubStateIds::Waiting);
    }
}


