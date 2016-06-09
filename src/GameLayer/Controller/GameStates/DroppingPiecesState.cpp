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
    if (!pAnimationState || pAnimationState->IsFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces);
    }
}


void FallingPiecesState::OnUpdate(double dt)
{
    if (pAnimationState->IsFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces);
    }
}

void FallingPiecesState::OnExit()
{
    mControllerRef.UpdateBoard(mDestroyedPieces, mPiecesToMove);
}