//
//  ColumnsBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "ColumnsBoard.hpp"

bool ColumnsBoard::MovePlayerBlock(TilePosition newPos)
{
    if (!CanMovePlayerBlockTo(newPos)) return false;

    UpdateBoardStateWithPlayerBlockAtPosition(newPos);
    
    return true;
}


bool ColumnsBoard::CanMovePlayerBlockTo(TilePosition newPos) const
{   
    // Check if we can move
    for(auto i = 0; i <_playerBlock.size(); ++i)
    {
        if(_boardTiles[newPos.x][newPos.y + i] <= 0)
        {
            return false;
        }
    }
    
    return true;
}

void ColumnsBoard::UpdateBoardStateWithPlayerBlockAtPosition(TilePosition newPos)
{
    // Move block and update board state
    for(auto i = 0; i <_playerBlock.size(); ++i)
    {
        _boardTiles[_playerBlockPosition.x][_playerBlockPosition.y+i] = EBoardPieces::Empty;
        
        _boardTiles[newPos.x][newPos.y + i] = _playerBlock[i];
    }
    
    
}