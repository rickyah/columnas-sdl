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
        if (!GenericBoard::IsPositionInsideBoardBounds(newPos)) return false;
        
        if(!IsPositionInsidePlayerBlock(newPos) && _boardTiles[newPos.row][newPos.col] != ESpecialBoardPieces::Empty)
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
        _boardTiles[_playerBlockPosition.row + i][_playerBlockPosition.col] = ESpecialBoardPieces::Empty;
    }
    
    for(auto i = 0; i <_playerBlock.size(); ++i)
    {        
        _boardTiles[newPos.row + i][newPos.col] = _playerBlock[i];
    }
    
    
    _playerBlockPosition = newPos;
}

TilesSet ColumnsBoard::FindPiecesToDestroy() const
{
    _listPiecesToDestroy.clear();
    
    for(auto i = 0; i <_playerBlock.size(); ++i)
    {
        auto set = GetAllAdjacentTiles(_playerBlockPosition.row + i, _playerBlockPosition.col);
        if (set.size() > (_numPiecesToDestroy -1) )
        {
            _listPiecesToDestroy.insert(set.begin(), set.end());   
        }
    }
 
    return _listPiecesToDestroy;
}

void ColumnsBoard::RemovePieces(const TilesSet &pieces)
{
    for(auto piece: pieces)
    {
        _boardTiles[piece.row][piece.col] = ESpecialBoardPieces::Empty;
    }
}


TilesMovementSet ColumnsBoard::FindAllPiecesToMove() const
{
    // Search in all columns
    _tmpColumnsToCheck.clear();
    
    for(int r = 0; r < columns(); ++r) _tmpColumnsToCheck.insert(r);
    
    return FindPiecesToMoveInColumns(_tmpColumnsToCheck);
}


TilesMovementSet ColumnsBoard::FindPiecesToMoveInSubset(const TilesSet &destroyedPieces) const
{
    _tmpColumnsToCheck.clear();
    
    // slight optimization: only check columns where one piece got destroyed as are the ones that can have
    // falling blocks
    std::transform(destroyedPieces.begin(),
                   destroyedPieces.end(),
                   std::inserter(_tmpColumnsToCheck, _tmpColumnsToCheck.begin()),
                   [](const TilePosition & p) {
                       return p.col;
                   });

    return FindPiecesToMoveInColumns(_tmpColumnsToCheck);

}

void ColumnsBoard::MovePieces(const TilesMovementSet &pieces)
{
    
    for(int i = 0; i < pieces.size(); ++i)
    {
        TileMovement move = pieces[i];
        TileType valueToMove = _boardTiles[move.from.row][move.from.col];
        _boardTiles[move.from.row][move.from.col] = ESpecialBoardPieces::Empty;
        _boardTiles[move.to.row][move.to.col] = valueToMove;
    }
    
}

TilesMovementSet ColumnsBoard::FindPiecesToMoveInColumns(const std::unordered_set<uint8_t> &columnsToCheck) const
{
    _listPiecesToFall.clear();
    
    for(int currentCol: columnsToCheck)
    {
        
        // indexes to the positions marking where a piece should move FROM, TO the new position
        int fromRowIdx, toRowIdx;
        fromRowIdx = toRowIdx = rows()-1;
        
        // Search the first empty tile in the column starting from the bottom
        // Thats the first position we can potentially move a piece TO
        while(toRowIdx > 0 && _boardTiles[toRowIdx][currentCol] != ESpecialBoardPieces::Empty)
        {
            toRowIdx--;
        }
        
        // search in the upper tile above the first empty one
        fromRowIdx = toRowIdx-1;
        
        while(fromRowIdx > 0 && toRowIdx > 0)
        {
            // Update the FROM index until we found a non empty tile that can be moved
            if(_boardTiles[fromRowIdx][currentCol] == ESpecialBoardPieces::Empty)
            {
                fromRowIdx--;
            }
            else
            {
                // If the tile to move is not empty, we got a piece that can be moved FROM an index TO another index
                // But only if the TO index points to an empty tile
                if (fromRowIdx != toRowIdx && _boardTiles[toRowIdx][currentCol] == ESpecialBoardPieces::Empty)
                {
                    
                    _listPiecesToFall.push_back(TileMovement(TilePosition(fromRowIdx, currentCol), TilePosition(toRowIdx, currentCol)));
                    
                    fromRowIdx--;
                }
                
                // If the TO index points to a non empty tile, we just look in the row above
                toRowIdx--;
            }
        }
    }
    
    return _listPiecesToFall;
}

