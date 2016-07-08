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
    Logger::log.Info(__PRETTY_FUNCTION__);
    auto tuple = mControllerRef.StartFallingPieces(mDestroyedPieces);
    
    pPiecesToMove = std::get<ColumnsGameController::destroyedPieces>(tuple);
    pAnimationState = std::get<ColumnsGameController::animationState>(tuple);
    
    
    if (mPiecesToMove.size() <= 0)
    {
        ExtractPositionOfDroppedPieces(mDestroyedPieces, mPiecesToMove);
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces, std::make_shared<DestroyPiecesStateArgs>(mDestroyedPieces));
    }
}

void FallingPiecesState::OnUpdate(float dt)
{
    if (!pAnimationState || pAnimationState->hasFinished())
    {
        ExtractPositionOfDroppedPieces(mDestroyedPieces, mPiecesToMove);
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces,std::make_shared<DestroyPiecesStateArgs>(mDestroyedPieces));
        return;
    }
}

void FallingPiecesState::OnExit()
{
    Logger::log.Info(__PRETTY_FUNCTION__);
    mControllerRef.UpdateBoardMakePiecesFall(mPiecesToMove);
    mDestroyedPieces.clear();
    mPiecesToMove.clear();
}

void FallingPiecesState::ExtractPositionOfDroppedPieces(TilesSet &destroyedPieces, const TilesMovementSet &piecesMovedDown)
{
    destroyedPieces.clear();
    std::transform(piecesMovedDown.begin(), piecesMovedDown.end(), std::inserter(destroyedPieces, destroyedPieces.begin()), [](const TileMovement &tileToMove) {
        return tileToMove.to;
    });
    
}
