//
//  ColumnsBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "ColumnsBoard.hpp"

bool ColumnsBoard::CanMovePlayerBlockDown(const PlayerBlock &playerBlock) const
{
    return CanMovePlayerBlockTo({1,0}, playerBlock);
}


bool ColumnsBoard::CanMovePlayerBlockLeft(const PlayerBlock &playerBlock) const
{
    return CanMovePlayerBlockTo({0,-1}, playerBlock);
}

bool ColumnsBoard::CanMovePlayerBlockRight(const PlayerBlock &playerBlock) const
{
    return CanMovePlayerBlockTo({0,1}, playerBlock);
}

bool ColumnsBoard::IsGameOverConditionFullfilled() const
{
    for(int row = 0; row < mNumFirstRowsForGameOver; ++row)
    {
        auto currentRow = mBoardTiles[row];
        if (std::find_if(currentRow.begin(), currentRow.end(), [](TileType t) { return t > 0;}) != currentRow.end())
        {
            return true;
        }
    }
    
    return false;
}

void ColumnsBoard::ConsolidatePlayerBlock(const PlayerBlock &playerBlock)
{
    auto position = playerBlock.position();
    for(size_t  i = 0; i < playerBlock.size(); ++i)
    {
        mBoardTiles[position.row + i][position.col] = playerBlock[i];
    }
}

TilesMovementSet ColumnsBoard::FindAllPiecesToMove() const
{
    // Search in all columns
    mTmpColumnsToCheck.clear();
    
    for(auto r = 0; r < columns(); ++r) mTmpColumnsToCheck.insert(r);
    
    return FindPiecesToMoveInColumns(mTmpColumnsToCheck);
}

TilesSet ColumnsBoard::FindPiecesToDestroy(TilesSet positionsToSearch) const
{
    mListPiecesToDestroy.clear();
    
    for(const auto& position: positionsToSearch)
    {
        auto set = GetAllAdjacentTiles(position.row, position.col, [this](std::unordered_set<TilePosition> positions) {
            return FindAdjacentPiecesFilterFunc(positions);
        });
        
        // sustract 1 as the GetAllAdjacentTiles does not takes into account the piece in the position
        // you search
        if (set.size() > (mNumEqualPiecesToDestroy -1) )
        {
            mListPiecesToDestroy.insert(set.begin(), set.end());
        }
    }
    
    return mListPiecesToDestroy;
}

TilesMovementSet ColumnsBoard::FindPiecesToMoveInSubset(const TilesSet &destroyedPieces) const
{
    mTmpColumnsToCheck.clear();
    
    // slight optimization: only check columns where one piece got destroyed as are the ones that can have
    // falling blocks
    std::transform(destroyedPieces.begin(),
                   destroyedPieces.end(),
                   std::inserter(mTmpColumnsToCheck, mTmpColumnsToCheck.begin()),
                   [](const TilePosition & p) {
                       return p.col;
                   });
    
    return FindPiecesToMoveInColumns(mTmpColumnsToCheck);
    
}


////////////////////////////////////////////////////////////////////////
// Private Helpers


bool ColumnsBoard::CanMovePlayerBlockTo(TileOffset offset, const PlayerBlock &playerBlock) const
{
    // Check if we can move
    TileCoordinate firstRow = std::floor(playerBlock.position().row + offset.rowOffset);
    TileCoordinate lastRow;
    
    // Special case: when moving to the side if we are in the middle of a tile we might collide to a
    // non-empty tile, so we need to check one tile down ahead
    if (offset.rowOffset != 0)
    {
        lastRow = std::floor(playerBlock.position().row + playerBlock.size() + offset.rowOffset);
    }
    else
    {
        lastRow = std::ceil(playerBlock.position().row + playerBlock.size() + offset.rowOffset);
    }
    
    TileCoordinate newCol = playerBlock.position().col + offset.colOffset;
    
    for(unsigned short rowIdx = firstRow; rowIdx < lastRow; ++rowIdx)
    {
        if (!GenericBoard::IsPositionInsideBoardBounds({rowIdx, newCol}))
        {
             return false;
        }
        
        if(mBoardTiles[rowIdx][newCol] != ESpecialBoardPieces::Empty)
        {
            return false;
        }
    }
    
    return true;
}


TilesMovementSet ColumnsBoard::FindPiecesToMoveInColumns(const std::unordered_set<TileCoordinate> &columnsToCheck) const
{
    mListPiecesToFall.clear();
    
    for(int currentCol: columnsToCheck)
    {
        
        // indexes to the positions marking where a piece should move FROM, TO the new position
        int fromRowIdx, toRowIdx;
        toRowIdx = rows()-1;
        
        // Search the first empty tile in the column starting from the bottom
        // Thats the first position we can potentially move a piece TO
        while(toRowIdx > 0 && mBoardTiles[toRowIdx][currentCol] != ESpecialBoardPieces::Empty)
        {
            toRowIdx--;
        }
        
        // search in the upper tile above the first empty one
        fromRowIdx = toRowIdx-1;
        
        while(fromRowIdx > 0 && toRowIdx > 0)
        {
            // Update the FROM index until we found a non empty tile that can be moved
            if(mBoardTiles[fromRowIdx][currentCol] == ESpecialBoardPieces::Empty)
            {
                fromRowIdx--;
            }
            else
            {
                // If the tile to move is not empty, we got a piece that can be moved FROM an index TO another index
                // But only if the TO index points to an empty tile
                if (fromRowIdx != toRowIdx /*&& mBoardTiles[toRowIdx][currentCol] == ESpecialBoardPieces::Empty*/)
                {
                    mListPiecesToFall.push_back(TileMovement(TilePosition(fromRowIdx, currentCol), TilePosition(toRowIdx, currentCol)));
                    
                    fromRowIdx--;
                }
                
                // If the TO index points to a non empty tile, we just look in the row above
                toRowIdx--;
            }
        }
    }
    
    return mListPiecesToFall;
}



bool ColumnsBoard::FindAdjacentPiecesFilterFunc(std::unordered_set<TilePosition> positions) const
{
    return positions.size() < (mNumEqualPiecesToDestroy -1);
}
