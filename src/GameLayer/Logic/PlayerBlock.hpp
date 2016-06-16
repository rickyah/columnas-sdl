//
//  PlayerBlock.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#ifndef PlayerBlock_h
#define PlayerBlock_h

#include "DataTypes.hpp"

class ColumnsBoard;



struct PositionF
{
    float row;
    float col;
    
    PositionF():PositionF(0,0) {}
    
    PositionF(float row_, float col_):row(row_), col(col_) {}
    
};

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
        mPieces = newPieces;
        mFirstPieceIdx = 0;
    }
    
    void position(TilePosition newPosition){mPosition.row = newPosition.row; mPosition.row = newPosition.col;}
    void position(PositionF newPosition){mPosition = newPosition;}
    PositionF& position() { return mPosition;}
    PositionF position() const { return mPosition;}
    
    TilesSet occupiedPositions() const
    
    {
        mOccupiedPositionsTmp.clear();
        
        for (size_t i = 0; i < size(); ++i)
        {
            mOccupiedPositionsTmp.insert(TilePosition(static_cast<TileCoordinate>(mPosition.row + i),
                                                      static_cast<TileCoordinate>(mPosition.col)));
        }
        
        return mOccupiedPositionsTmp;
    }
    
    /*
     * Returns the piece with the given index, taking into account that
     * the pieces could be cycled
     */
    const TileType& operator[](std::size_t idx) const
    {
        return mPieces[(mFirstPieceIdx + idx) % size()];
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
        ++mFirstPieceIdx;
        mFirstPieceIdx = mFirstPieceIdx % size();
    }
    
    size_t size() const { return mPieces.size(); }
    
private:
    PositionF mPosition;
    std::vector<TileType> mPieces;
    size_t mFirstPieceIdx = 0;
    mutable TilesSet mOccupiedPositionsTmp;
    PlayerBlock(const PlayerBlock &);
    PlayerBlock& operator=(const PlayerBlock &);
};



#endif /* PlayerBlock_h */
