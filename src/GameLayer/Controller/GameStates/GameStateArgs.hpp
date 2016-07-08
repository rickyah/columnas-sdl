//
//  GameStateArgs.h
//  Columns
//
//  Created by Ricardo Amores Hernández on 9/6/16.
//
//

#ifndef GameStateArgs_h
#define GameStateArgs_h

#include "ColumnsBoard.hpp"

/*
 * States arguments pass from the RemovingPiecesState to the FallingPiecesState
 */
struct PiecesSetStateArgs : IStateArgs
{
    PiecesSetStateArgs(TilesSet pieces_):pieces(pieces_){}

    TilesSet pieces;
};

#endif /* GameStateArgs_h */
