//
//  RemovingPiecesState.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#include "RemovingPiecesState.hpp"



void RemovingPiecesState::OnEnter()
{
    auto tuple = mControllerRef.StartDestroyingPieces();
    
    mPiecesToDestroy = std::get<ColumnsGameController::destroyedPieces>(tuple);
    pAnimationState = std::get<ColumnsGameController::animationState>(tuple);
    
    if (!pAnimationState || pAnimationState->IsFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    }
}

void RemovingPiecesState::OnUpdate(double dt)
{
    if (pAnimationState->IsFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Falling_Pieces,
                      std::make_shared<DestroyPiecesStateArgs>(mPiecesToDestroy));
    }
}
