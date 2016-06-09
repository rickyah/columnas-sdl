//
//  DroppingPiecesState.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#include "DroppingPiecesState.hpp"

void FallingPiecesState::OnSetArgs(std::shared_ptr<IStateArgs> pArgs)
{
    if (pArgs)
    {
        mDestroyedPieces = std::static_pointer_cast<DestroyPiecesStateArgs>(pArgs)->destroyedPieces;
    }
}

void FallingPiecesState::OnEnter()
{
    SDL_Log(__PRETTY_FUNCTION__);
    auto tuple = mControllerRef.StartFallingPieces(mDestroyedPieces);
    
    mPiecesToMove = std::get<ColumnsGameController::destroyedPieces>(tuple);
    pAnimationState = std::get<ColumnsGameController::animationState>(tuple);
    
    
    if (mPiecesToMove.size() <= 0)
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces);
    }
}


void FallingPiecesState::OnUpdate(double dt)
{
    if (!pAnimationState || pAnimationState->IsFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces);
        return;
    }
}

void FallingPiecesState::OnExit()
{
    SDL_Log(__PRETTY_FUNCTION__);
    mControllerRef.UpdateBoardMakePiecesFall(mPiecesToMove);
    mDestroyedPieces.clear();
    mPiecesToMove.clear();
}