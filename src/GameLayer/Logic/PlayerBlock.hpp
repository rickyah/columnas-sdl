//
//  PlayerBlock.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#ifndef PlayerBlock_h
#define PlayerBlock_h

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

#endif /* PlayerBlock_h */
