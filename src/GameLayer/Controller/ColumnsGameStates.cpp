//
//  ColumnsGameStates.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#include "ColumnsGameStates.hpp"


void MovingPiecesState::OnUpdate(double dt)
{
    static int32_t passedTime = 0;
    passedTime += dt;
    
    bool endGame = false;
    int timePerDropMs = 1000;
    if (passedTime >= timePerDropMs)
    {
        //        SDL_Log("elapsed: %d, passed %d", time.elapsedMs, passedTime);
        if(!mControllerRef.MovePlayerDown())
        {
            if(! mControllerRef.ResetPlayerBlock())
            {
                
            }
        }
        
        
        // update view
        passedTime = std::max(0, passedTime -timePerDropMs);
    }

}