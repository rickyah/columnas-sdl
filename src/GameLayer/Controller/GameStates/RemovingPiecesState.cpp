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
    
    if (mPiecesToDestroy.size() <= 0)
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    }
    
    SDL_Log(__PRETTY_FUNCTION__);
}

void RemovingPiecesState::OnUpdate(double dt)
{
    if (mPiecesToDestroy.size() <= 0)
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
        return;
    }
    
    if (!pAnimationState || pAnimationState->IsFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Falling_Pieces,
                      std::make_shared<DestroyPiecesStateArgs>(mPiecesToDestroy));
    }
}

void RemovingPiecesState::OnExit()
{
    mControllerRef.UpdateBoardDestroyPieces(mPiecesToDestroy);
    mPiecesToDestroy.clear();
    SDL_Log(__PRETTY_FUNCTION__);
}
