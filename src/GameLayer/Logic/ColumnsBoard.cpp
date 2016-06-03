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

void ColumnsBoard::DestroyPieces(TilesSet pieces)
{
    for(auto piece: pieces)
    {
        _boardTiles[piece.row][piece.col] = ESpecialBoardPieces::Empty;
    }
}

TilesMovementSet ColumnsBoard::FindPiecesToMove(TilesSet destroyedPieces) const
{
    static std::vector<TilePosition> sortedDestroyedPieces;
    _listPiecesToFall.from.clear();
    _listPiecesToFall.to.clear();
    
    if (destroyedPieces.size() == 0) return _listPiecesToFall;
    
    std::copy(destroyedPieces.begin(), destroyedPieces.end(), std::back_inserter(sortedDestroyedPieces));
    std::sort(sortedDestroyedPieces.begin(), sortedDestroyedPieces.end(), [](const TilePosition & a, const TilePosition & b) -> bool{
        if (a.col == b.col) return a.row > b.row;
        return a.col < b.col;
    });
    
    
    int currentCol = -1;
    auto idxPlaceRow = sortedDestroyedPieces[0].row;
    
    for(int i = 0; i < sortedDestroyedPieces.size(); ++i)
    {
        auto pos =  sortedDestroyedPieces[i];
        
        if (currentCol == pos.col) continue;
        
        
        currentCol = sortedDestroyedPieces[i].col;
        idxPlaceRow = sortedDestroyedPieces[i].row;
        
        
        for(int j = idxPlaceRow; j > 0; --j)
        {
            if(_boardTiles[j][currentCol] != ESpecialBoardPieces::Empty)
            {
                _listPiecesToFall.from.push_back(TilePosition(j, currentCol));
                _listPiecesToFall.to.push_back(TilePosition(idxPlaceRow, currentCol));
                idxPlaceRow--;
            }
        }
    }

    return _listPiecesToFall;
}


void ColumnsBoard::MovePieces(TilesMovementSet pieces)
{

    for(int i = 0; i < pieces.size(); ++i)
    {
        TileType valueToMove = _boardTiles[pieces.from[i].row][pieces.from[i].col];
        _boardTiles[pieces.from[i].row][pieces.from[i].col] = ESpecialBoardPieces::Empty;
        _boardTiles[pieces.to[i].row][pieces.to[i].col] = valueToMove;
    }
    
}