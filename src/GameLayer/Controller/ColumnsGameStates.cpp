//
//  ColumnsGameStates.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#include "ColumnsGameStates.hpp"
#include <cmath>

MovingPiecesState::MovingPiecesState(const ColumnsGameFSM &fsm,
                                     ColumnsGameController &controller,
                                     EventQueue & eventQueue)
    :ColumnsGameState(fsm, controller),
    mEventQueueRef(eventQueue),
    mOnTouchEventDlg(EventListenerDelegate(std::bind(&MovingPiecesState::OnTouchEvent, this, std::placeholders::_1)))
{
    
}

void MovingPiecesState::OnUpdate(double dt)
{
    SDL_Log("Logic frame");
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
    static AppTouch_Event::TouchMotion motion;
    auto ptr = std::static_pointer_cast<AppTouch_Event>(event);
    auto args = ptr->args();
    
   
    if (args.type == AppTouch_Event::ETouchType::Motion)
    {
        motion = FilterMotion(args.motion);
        
        if(motion.dx > 0)
        {
            mControllerRef.MoveRight();
        }
        else if(motion.dx < 0)
        {
            mControllerRef.MoveLeft();
        }
        else if (motion.dy > 0)
        {
            mControllerRef.MoveDown();
        }
    }
    else if (args.type == AppTouch_Event::ETouchType::Up)
    {
        mControllerRef.PermutePlayerBlockPieces();
    }

}

AppTouch_Event::TouchMotion MovingPiecesState::FilterMotion(AppTouch_Event::TouchMotion motion) const
{
    
    if (std::abs(motion.dy) > std::abs(motion.dx))
    {
        motion.dx = 0;
    }
    
    return motion;
}