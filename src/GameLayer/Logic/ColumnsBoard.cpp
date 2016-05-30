//
//  ColumnsBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "ColumnsBoard.hpp"


bool ColumnsBoard::MovePlayerBlockRight()
{
 
    return MovePlayerBlockToPosition(TilePosition(_playerBlockPosition.row, _playerBlockPosition.col+1));
}

bool ColumnsBoard::MovePlayerBlockLeft()
{
    return MovePlayerBlockToPosition(TilePosition(_playerBlockPosition.row, _playerBlockPosition.col-1));
}

bool ColumnsBoard::MovePlayerBlockDown()
{
    return MovePlayerBlockToPosition(TilePosition(_playerBlockPosition.row+1, _playerBlockPosition.col));
}


bool ColumnsBoard::MovePlayerBlockToPosition(const TilePosition &newPos)
{
    if (!CanMovePlayerBlockTo(newPos)) return false;

    UpdateBoardStateWithPlayerBlockAtPosition(newPos);
    
    return true;
}

bool ColumnsBoard::IsPositionInsidePlayerBlock(TilePosition pos) const
{
    return pos.col == _playerBlockPosition.col
        && pos.row >= _playerBlockPosition.row
        && pos.row < _playerBlockPosition.row + _playerBlock.size();
}

bool ColumnsBoard::CanMovePlayerBlockTo(TilePosition newPos) const
{
    // Check if we can move
    for(auto i = 0; i <_playerBlock.size(); ++i)
    {
        if (!IsPositionInsideBoardBounds(newPos)) return false;
        
        if(!IsPositionInsidePlayerBlock(newPos) && _boardTiles[newPos.row][newPos.col] != EBoardPieces::Empty)
        {
            return false;
        }
        
        newPos.row += 1;
    }
    
    return true;
}

void ColumnsBoard::ResetPlayerBlock(const std::vector<TileType> &pieces)
{
    _playerBlock.SetNewPieces(pieces);
    
    UpdateBoardStateWithPlayerBlockAtPosition(_playerBlockInitialPosition);
}

void ColumnsBoard::MovePlayerBlockPieces()
{
    _playerBlock.MovePieces();
    UpdateBoardStateWithPlayerBlockAtPosition(_playerBlockPosition);
}

void ColumnsBoard::UpdateBoardStateWithPlayerBlockAtPosition(const TilePosition &newPos)
{
    // Move block and update board state
    for(auto i = 0; i <_playerBlock.size(); ++i)
    {
        _boardTiles[_playerBlockPosition.row + i][_playerBlockPosition.col] = EBoardPieces::Empty;
        
        _boardTiles[newPos.row + i][newPos.col] = _playerBlock[i];
    }
    
    _playerBlockPosition = newPos;
}