//
//  ColumnsBoard.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef ColumnsBoard_hpp
#define ColumnsBoard_hpp

#include <vector>

#include "GenericBoard.hpp"
#include "PlayerBlock.hpp"


class ColumnsBoard : public GenericBoard
{
public:
    explicit ColumnsBoard(BoardState state):GenericBoard(state){}
    
    explicit ColumnsBoard(uint8_t rows, uint8_t columns):GenericBoard(rows, columns){}
    
    void InitPlayerBlockInitialPosition(TilePosition pos) { mPlayerBlockInitialPosition = pos; }
    void InitNumEqualPiecesToDestroy(uint8_t numPieces = 3) { mNumEqualPiecesToDestroy = numPieces; }
    void InitNumRowsGameOver(uint8_t numRows = 3) { mNumFirstRowsForGameOver = numRows; }
    
    
    bool ResetPlayerBlock(const std::vector<TileType> &pieces);
    
    bool MovePlayerBlockLeft();
    bool MovePlayerBlockRight();
    bool MovePlayerBlockDown();
    void MovePlayerBlockPieces();
    
    
    uint8_t numFirstRowsForGameOver() const { return mNumFirstRowsForGameOver; }
    bool CanMovePlayerBlockDown() const;
    bool IsGameOverConditionFullfilled() const;
    TilesSet FindPiecesToDestroy() const;
    TilesMovementSet FindAllPiecesToMove() const;
    TilesMovementSet FindPiecesToMoveInSubset(const TilesSet &) const;
    
private:
    uint8_t mNumEqualPiecesToDestroy;
    uint8_t mNumFirstRowsForGameOver;
    PlayerBlock mPlayerBlock;
    TilePosition mPlayerBlockPosition;
    TilePosition mPlayerBlockInitialPosition;
    
    mutable TilesSet mListPiecesToDestroy;
    mutable TilesMovementSet mListPiecesToFall;
    mutable std::unordered_set<TileCoordinate> mTmpColumnsToCheck;
    
    TilesMovementSet FindPiecesToMoveInColumns(const std::unordered_set<TileCoordinate> &) const;
    bool FindAdjacentPiecesFilterFunc(std::unordered_set<TilePosition> positions) const;
    
    bool IsPositionInsidePlayerBlock(TilePosition pos) const;
    bool CanMovePlayerBlockTo(TilePosition newPos) const;
    bool MovePlayerBlockToPosition(const TilePosition &newPos);
    void UpdateBoardStateWithPlayerBlockAtPosition(const TilePosition &newPos);
    
    
};


#endif /* ColumnsBoard_hpp */
