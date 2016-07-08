//
//  DroppingPiecesState.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#include "FallingPiecesState.hpp"

void FallingPiecesState::OnSetArgs(std::shared_ptr<IStateArgs> pArgs)
{
    if (auto args = std::static_pointer_cast<PiecesSetStateArgs>(pArgs))
    {
        mDestroyedPieces = args->pieces;
    }
}

void FallingPiecesState::OnEnter()
{
    Logger::log.Info(__PRETTY_FUNCTION__);
    
    if (mDestroyedPieces.size() <= 0)
    {
        mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
        return;
    }
    
    mControllerRef.StartFallingPieces(mDestroyedPieces, [this](TilesMovementSet &piecesToMove){
        TilesSet movedPieces;
        ExtractPositionOfDroppedPieces(movedPieces, piecesToMove);
        mFSM.ChangeTo(EColumnsGameStatesIds::Removing_Pieces, std::make_shared<PiecesSetStateArgs>(movedPieces));
    });
}


void FallingPiecesState::OnExit()
{
    Logger::log.Info(__PRETTY_FUNCTION__);
    
    mDestroyedPieces.clear();
}

void FallingPiecesState::ExtractPositionOfDroppedPieces(TilesSet &destroyedPieces, const TilesMovementSet &piecesMovedDown)
{
    destroyedPieces.clear();
    std::transform(piecesMovedDown.begin(), piecesMovedDown.end(), std::inserter(destroyedPieces, destroyedPieces.begin()), [](const TileMovement &tileToMove) {
        return tileToMove.to;
    });
    
}
