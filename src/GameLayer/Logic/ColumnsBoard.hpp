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
    explicit ColumnsBoard(uint8_t width, uint8_t height):GenericBoard(width, height){}
    
    void SetPlayerBlockInitialPosition(TilePosition pos) { mPlayerBlockInitialPosition = pos; }
    void SetNumEqualPiecesToDestroy(uint8_t numPieces) { mNumEqualPiecesToDestroy = numPieces; }
    void ResetPlayerBlock(const std::vector<TileType> &pieces);
    bool MovePlayerBlockLeft();
    bool MovePlayerBlockRight();
    bool MovePlayerBlockDown();
    
    void MovePlayerBlockPieces();
    
    void Update();
    
    TilesSet FindPiecesToDestroy() const;

    TilesMovementSet FindAllPiecesToMove() const;
    TilesMovementSet FindPiecesToMoveInSubset(const TilesSet &) const;
    
    
private:
    uint8_t mNumEqualPiecesToDestroy;
    PlayerBlock mPlayerBlock;
    TilePosition mPlayerBlockPosition;
    TilePosition mPlayerBlockInitialPosition;
    
    mutable TilesSet mListPiecesToDestroy;
    mutable TilesMovementSet mListPiecesToFall;
    mutable std::unordered_set<uint8_t> mTmpColumnsToCheck;
    
    TilesMovementSet FindPiecesToMoveInColumns(const std::unordered_set<uint8_t> &) const;
    bool IsPositionInsidePlayerBlock(TilePosition pos) const;
    bool CanMovePlayerBlockTo(TilePosition newPos) const;
    bool MovePlayerBlockToPosition(const TilePosition &newPos);
    void UpdateBoardStateWithPlayerBlockAtPosition(const TilePosition &newPos);
};


#endif /* ColumnsBoard_hpp */
