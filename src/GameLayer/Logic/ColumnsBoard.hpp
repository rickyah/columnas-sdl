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


enum ESpecialBoardPieces {
    Disabled = -1,
    Empty = 0,
};

class ColumnsBoard;


/*
 * Represents the block of the player that contains a set
 * of pieces that can be permuted in a cycle
 *
 * Use a for loop to iterate between the pieces
 */
class PlayerBlock
{
public:
    
    PlayerBlock(){};
    /*
     * define a new set of pieces to use
     */
    void SetNewPieces(const std::vector<TileType> &newPieces)
    {
        _pieces = newPieces;
        _firstPieceIdx = 0;
    }
    
    /*
     * Returns the piece with the given index, taking into account that
     * the pieces could be cycled
     */
    const TileType& operator[](std::size_t idx) const
    {
        return _pieces[(_firstPieceIdx + idx) % size()];
    }
    
    /*
     * Cycles the pices so the next becomes the first, and the previously first become the last:
     * [1,2,3]
     * MovePieces()
     * [2,3,1]
     * MovePieces()
     * [3,1,2]
     */
    void MovePieces()
    {
        ++_firstPieceIdx;
        _firstPieceIdx = _firstPieceIdx % size();
    }
    
    size_t size() const { return _pieces.size(); }
    
private:
    std::vector<TileType> _pieces;
    size_t _firstPieceIdx = 0;
    
    PlayerBlock(const PlayerBlock &);
    PlayerBlock& operator=(const PlayerBlock &);
};

typedef std::unordered_set<TilePosition> TilesSet;
struct TilesMovementSet
{
    std::vector<TilePosition> from;
    std::vector<TilePosition> to;
    
    size_t size() { return from.size(); }
};

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
    void DestroyPieces(const TilesSet &pieces);
    TilesMovementSet FindPiecesToMove(const TilesSet &) const;
    void MovePieces(const TilesMovementSet &pieces);

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
