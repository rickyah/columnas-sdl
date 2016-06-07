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
                                     EventQueue & eventQueue)
:BaseColumnsGameState(fsm, controller),
mEventQueueRef(eventQueue),
mOnTouchEventDlg(EventListenerDelegate(std::bind(&MovingPiecesState::OnTouchEvent, this, std::placeholders::_1)))
{
    
}

void MovingPiecesState::OnUpdate(double dt)
{
    static int32_t passedTime = 0;
    passedTime += dt;
    
    bool endGame = false;
    int timePerDropMs = 1000;
    if (passedTime >= timePerDropMs)
    {
        //        SDL_Log("elapsed: %d, passed %d", time.elapsedMs, passedTime);
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

void MovingPiecesState::OnTouchEvent(std::shared_ptr<IEventData> event)
{
    static AppTouch_Event::TouchMotion filteredMotion;
    static double motionAcc;
    static int frameCount;
    static uint8_t moveDirection = 0;
    
    auto ptr = std::static_pointer_cast<AppTouch_Event>(event);
    auto args = ptr->args();
    filteredMotion = FilterMotion(args.motion);
    
    frameCount++;
    
    switch(mMovementState)
    {
        case MovementState::Waiting:
            if (args.type == AppTouch_Event::ETouchType::Down)
            {
                mMovementState = MovementState::Accept_Input;
                frameCount = 0;
            }
            break;
            
        case MovementState::Accept_Input:
            if (args.type == AppTouch_Event::ETouchType::Up)
            {
                mMovementState = MovementState::Waiting;
            }
            
            if (HasMotion(filteredMotion))
            {
                if (filteredMotion.dy > filteredMotion.dx)
                {
                    mMovementState = MovementState::Moving_Down;
                    moveDirection = 0;
                }
                else if(filteredMotion.dx > 0)
                {
                    mMovementState = MovementState::Moving_Right;
                    moveDirection = 1;
                }
                else if(filteredMotion.dx < 0)
                {
                    mMovementState = MovementState::Moving_Left;
                    moveDirection = -1;
                }
            }
            else if (frameCount > 5)
            {
                mMovementState = MovementState::Permuting;
                mControllerRef.PermutePlayerBlockPieces();
            }
            break;
            
        case MovementState::Moving_Left:
        case MovementState::Moving_Right:
        case MovementState::Moving_Down:
            if (args.type == AppTouch_Event::ETouchType::Up)
            {
                mMovementState = MovementState::Waiting;
                motionAcc = 0;
                moveDirection = 0;
            }
            
            if (moveDirection == 0) mControllerRef.MoveDown();
            else if(moveDirection < 0) mControllerRef.MoveLeft();
            else if(moveDirection > 0) mControllerRef.MoveRight();
            
            break;
            
        case MovementState::Permuting:
            if (args.type == AppTouch_Event::ETouchType::Up)
            {
                mMovementState = MovementState::Waiting;
            }
            break;
    }
    
    //    if (args.type == AppTouch_Event::ETouchType::Motion)
    //    {
    //        motion = FilterMotion(args.motion);
    //
    //        if(motion.dx > 0)
    //        {
    //            mControllerRef.MoveRight();
    //        }
    //        else if(motion.dx < 0)
    //        {
    //            mControllerRef.MoveLeft();
    //        }
    //        else if (motion.dy > 0)
    //        {
    //            mControllerRef.MoveDown();
    //        }
    //    }
    //    else if (args.type == AppTouch_Event::ETouchType::Up)
    //    {
    //        mControllerRef.PermutePlayerBlockPieces();
    //    }
    
}

bool MovingPiecesState::HasMotion(AppTouch_Event::TouchMotion motion) const
{
    if (std::abs(motion.dx) > 0 || std::abs(motion.dy) > 0) return true;
    
    return false;
}

AppTouch_Event::TouchMotion MovingPiecesState::FilterMotion(AppTouch_Event::TouchMotion motion) const
{
    static const float EPSILON = 0.02;
    if (std::abs(motion.dx) < EPSILON)
    {
        motion.dx = 0;
    }
    if (motion.dy < 0 || std::abs(motion.dy) < EPSILON)
    {
        motion.dy = 0;
    }
    
    return motion;
}