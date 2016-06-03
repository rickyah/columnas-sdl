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
    
    void SetPlayerBlockInitialPosition(TilePosition pos) { _playerBlockInitialPosition = pos; }
    void SetNumEqualPiecesToDestroy(uint8_t numPieces) { _numPiecesToDestroy = numPieces; }
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
    uint8_t _numPiecesToDestroy;
    PlayerBlock _playerBlock;
    TilePosition _playerBlockPosition;
    TilePosition _playerBlockInitialPosition;
    
    mutable TilesSet _listPiecesToDestroy;
    mutable TilesMovementSet _listPiecesToFall;
    mutable std::unordered_set<uint8_t> _tmpColumnsToCheck;
    
    TilesMovementSet FindPiecesToMoveInColumns(const std::unordered_set<uint8_t> &) const;
    bool IsPositionInsidePlayerBlock(TilePosition pos) const;
    bool CanMovePlayerBlockTo(TilePosition newPos) const;
    bool MovePlayerBlockToPosition(const TilePosition &newPos);
    void UpdateBoardStateWithPlayerBlockAtPosition(const TilePosition &newPos);
};


#endif /* ColumnsBoard_hpp */
