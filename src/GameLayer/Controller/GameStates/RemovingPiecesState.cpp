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
    if (auto args = std::static_pointer_cast<PiecesSetStateArgs>(pArgs))
    {
        mPiecesToSearch = args->pieces;
    }
}


void RemovingPiecesState::OnEnter()
{
    mControllerRef.StartDestroyingPieces(mPiecesToSearch, [this](TilesSet &piecesToDestroy) {
        mFSM.ChangeTo(EColumnsGameStatesIds::Falling_Pieces, std::make_shared<PiecesSetStateArgs>(piecesToDestroy));
    });
 
}

void RemovingPiecesState::OnExit()
{
    mPiecesToSearch.clear();
}
