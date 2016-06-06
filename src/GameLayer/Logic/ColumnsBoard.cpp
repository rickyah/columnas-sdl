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
 
    return MovePlayerBlockToPosition(TilePosition(mPlayerBlockPosition.row, mPlayerBlockPosition.col+1));
}

bool ColumnsBoard::MovePlayerBlockLeft()
{
    return MovePlayerBlockToPosition(TilePosition(mPlayerBlockPosition.row, mPlayerBlockPosition.col-1));
}

bool ColumnsBoard::MovePlayerBlockDown()
{
    return MovePlayerBlockToPosition(TilePosition(mPlayerBlockPosition.row+1, mPlayerBlockPosition.col));
}


bool ColumnsBoard::MovePlayerBlockToPosition(const TilePosition &newPos)
{
    if (!CanMovePlayerBlockTo(newPos)) return false;

    UpdateBoardStateWithPlayerBlockAtPosition(newPos);
    
    return true;
}

bool ColumnsBoard::IsPositionInsidePlayerBlock(TilePosition pos) const
{
    return pos.col == mPlayerBlockPosition.col
        && pos.row >= mPlayerBlockPosition.row
        && pos.row < mPlayerBlockPosition.row + mPlayerBlock.size();
}

bool ColumnsBoard::CanMovePlayerBlockTo(TilePosition newPos) const
{
    // Check if we can move
    for(int i = 0; i < mPlayerBlock.size(); ++i)
    {
        if (!GenericBoard::IsPositionInsideBoardBounds(newPos)) return false;
        
        if(!IsPositionInsidePlayerBlock(newPos) && mBoardTiles[newPos.row][newPos.col] != ESpecialBoardPieces::Empty)
        {
            return false;
        }
        
        newPos.row += 1;
    }
    
    return true;
}

bool ColumnsBoard::ResetPlayerBlock(const std::vector<TileType> &pieces)
{
    // check that where the new block should be positioned is empty
    for(int i = 0; i < mPlayerBlock.size(); ++ i)
    {
        if (mBoardTiles[mPlayerBlockInitialPosition.row+i][mPlayerBlockInitialPosition.col] != ESpecialBoardPieces::Empty)
        {
            return false;
        }
    }

    mPlayerBlock.SetNewPieces(pieces);
    
    mPlayerBlockPosition = mPlayerBlockInitialPosition;
    
    UpdateBoardStateWithPlayerBlockAtPosition(mPlayerBlockInitialPosition);

    return true;
}

void ColumnsBoard::MovePlayerBlockPieces()
{
    mPlayerBlock.MovePieces();
    UpdateBoardStateWithPlayerBlockAtPosition(mPlayerBlockPosition);
}

void ColumnsBoard::UpdateBoardStateWithPlayerBlockAtPosition(const TilePosition &newPos)
{
    // Move block and update board state
    for(auto i = 0; i < mPlayerBlock.size(); ++i)
    {
        mBoardTiles[mPlayerBlockPosition.row + i][mPlayerBlockPosition.col] = ESpecialBoardPieces::Empty;
    }
    
    for(auto i = 0; i < mPlayerBlock.size(); ++i)
    {        
        mBoardTiles[newPos.row + i][newPos.col] = mPlayerBlock[i];
    }
    
    
    mPlayerBlockPosition = newPos;
}

TilesSet ColumnsBoard::FindPiecesToDestroy() const
{
    mListPiecesToDestroy.clear();
    
    for(auto i = 0; i < mPlayerBlock.size(); ++i)
    {
        auto set = GetAllAdjacentTiles(mPlayerBlockPosition.row + i, mPlayerBlockPosition.col);
        
        // sustract 1 as the GetAllAdjacentTiles does not takes into account the piece in the position
        // you search
        if (set.size() > (mNumEqualPiecesToDestroy -1) )
        {
            mListPiecesToDestroy.insert(set.begin(), set.end());
        }
    }
 
    return mListPiecesToDestroy;
}

TilesMovementSet ColumnsBoard::FindAllPiecesToMove() const
{
    // Search in all columns
    mTmpColumnsToCheck.clear();
    
    for(int r = 0; r < columns(); ++r) mTmpColumnsToCheck.insert(r);
    
    return FindPiecesToMoveInColumns(mTmpColumnsToCheck);
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

TilesMovementSet ColumnsBoard::FindPiecesToMoveInColumns(const std::unordered_set<uint8_t> &columnsToCheck) const
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
                if (fromRowIdx != toRowIdx && mBoardTiles[toRowIdx][currentCol] == ESpecialBoardPieces::Empty)
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

