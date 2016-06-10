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
    explicit ColumnsBoard(BoardState state, uint8_t numFirstRowsForGameOver, uint8_t numEqualPiecesToDestroy = 3)
        :GenericBoard(state),
         mNumFirstRowsForGameOver(numFirstRowsForGameOver),
         mNumEqualPiecesToDestroy(numEqualPiecesToDestroy)
    {}
    
    explicit ColumnsBoard(uint8_t rows, uint8_t columns, uint8_t numFirstRowsForGameOver, uint8_t numEqualPiecesToDestroy = 3)
        :GenericBoard(rows, columns),
        mNumFirstRowsForGameOver(numFirstRowsForGameOver),
        mNumEqualPiecesToDestroy(numEqualPiecesToDestroy)
    {}
    
    void numEqualPiecesToDestroy(uint8_t numPieces) { mNumEqualPiecesToDestroy = numPieces; }
    uint8_t numEqualPiecesToDestroy() { return mNumEqualPiecesToDestroy; }
    uint8_t numFirstRowsForGameOver() const { return mNumFirstRowsForGameOver; }
    
    bool CanMovePlayerBlockDown(const PlayerBlock &playerBlock) const;
    bool CanMovePlayerBlockLeft(const PlayerBlock &playerBlock) const;
    bool CanMovePlayerBlockRight(const PlayerBlock &playerBlock) const;
    bool IsGameOverConditionFullfilled() const;
    
    void ConsolidatePlayerBlock(const PlayerBlock &playerBlock);
    TilesMovementSet FindAllPiecesToMove() const;
    TilesSet FindPiecesToDestroy(TilesSet positionsToSearch) const;
    TilesMovementSet FindPiecesToMoveInSubset(const TilesSet &) const;

    
    
private:
    uint8_t mNumEqualPiecesToDestroy;
    uint8_t mNumFirstRowsForGameOver;
    // these are mutable as they only serve as temp holders
    mutable TilesSet mListPiecesToDestroy;
    mutable TilesMovementSet mListPiecesToFall;
    mutable std::unordered_set<TileCoordinate> mTmpColumnsToCheck;
    
    bool CanMovePlayerBlockTo(TileOffset offset, const PlayerBlock &playerBlock) const;
    TilesMovementSet FindPiecesToMoveInColumns(const std::unordered_set<TileCoordinate> &) const;
    bool FindAdjacentPiecesFilterFunc(std::unordered_set<TilePosition> positions) const;
};


#endif /* ColumnsBoard_hpp */
