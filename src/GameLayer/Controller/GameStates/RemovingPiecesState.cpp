//
//  RemovingPiecesState.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#include "RemovingPiecesState.hpp"


void RemovingPiecesState::OnSetArgs(std::shared_ptr<IStateArgs> pArgs)
{
    if (pArgs)
    {
        mPiecesToDestroy = std::static_pointer_cast<DestroyPiecesStateArgs>(pArgs)->destroyedPieces;
    }
}


void RemovingPiecesState::OnEnter()
{
    auto tuple = mControllerRef.StartDestroyingPieces(mPiecesToDestroy);
    
    mPiecesToDestroy = std::get<ColumnsGameController::destroyedPieces>(tuple);
    pAnimationState = std::get<ColumnsGameController::animationState>(tuple);
    
    
    if (mPiecesToDestroy.size() <= 0)
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    }
}

void RemovingPiecesState::OnUpdate(float dt)
{
    if (mPiecesToDestroy.size() <= 0)
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
        return;
    }
    
    if (!pAnimationState || pAnimationState->hasFinished())
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Falling_Pieces,
                      std::make_shared<DestroyPiecesStateArgs>(mPiecesToDestroy));
        
        pAnimationState = nullptr;
    }
}

void RemovingPiecesState::OnExit()
{
    mControllerRef.UpdateBoardDestroyPieces(mPiecesToDestroy);
    mPiecesToDestroy.clear();
}
