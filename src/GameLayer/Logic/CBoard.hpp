//
//  CBoard.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef CBoard_hpp
#define CBoard_hpp

#include <vector>
#include <stdio.h>


typedef short TileType;
typedef std::pair<uint8_t, uint8_t> TilePosition;
typedef std::vector< std::vector<TileType> > BoardState;

/*
 * Represents a generic game NxM board
 *
 * Allows getting and setting different tile values (int8_t types) in an agnostic way
 * Implements operations to search for adjacent tiles that have the same values
 */
class CBoard
{

public:

    // Allows using initialization_lists to set up a board, usefull for testing purposes
    // TODO: check if this should be changed to a Mememto pattern
    explicit CBoard(BoardState boardData);

    // Sets up an board of a given rows and columns, and an initial tile value.
    explicit CBoard(uint8_t rows, uint8_t columns, TileType initialValue = 0);

    uint8_t rows() const { return _boardTiles.size(); }
    uint8_t columns() const { return _boardTiles[0].size(); }

    std::vector<TilePosition> GetRowAdjacentTiles(uint8_t row, uint8_t col) const;
    std::vector<TilePosition> GetColAdjacentTiles(uint8_t row, uint8_t col) const;
    std::vector<TilePosition> GetRowAndColAdjacentTiles(uint8_t row, uint8_t col) const;
    std::vector<TilePosition> GetMainDiagonalAdjacentTiles(uint8_t row, uint8_t col) const;
    std::vector<TilePosition> GetSecondaryDiagonalAdjacentTiles(uint8_t row, uint8_t col) const;
    std::vector<TilePosition> GetDiagonalAdjacentTiles(uint8_t row, uint8_t col) const;
    std::vector<TilePosition> GetAllAdjacentTiles(uint8_t row, uint8_t col) const;

    static TilePosition MakeTilePos(uint8_t row, uint8_t col)
    {
        return std::pair<uint8_t, uint8_t>(row, col);
    }
private:

    /*
     * Search from a given position in the board for all the elements adjacent to that element with the same value
     *
     * Takes the given position POS and start searching for the board using the increaser pair to add/subtract values
     * to change the current search position. This allows using a single method to search rows, columns or diagonals
     * just changing the increaser's values.
     * Search uses the values increaser and -increaser until it reachs the limits of the board
     * or finds a value in the different from the initial value (the one found in the the given position)
     *
     * The position for the given element is NOT include in the result, to make combining different results easier
     * and avoid duplication of elements
     * The public method exposed to get the data DO include the position for the search starting position
     *
     * e.g searching in NxM board starting at position (2,3) with an increaser (1,0) searchs only in rows
     * advancing one position in each step, starting at (2,3) the sequence of elements iterated would be
     * (4,3) ... (M,3), (2,3), (1,3), (0,3)
     */
    std::vector<TilePosition> SearchAdjacentTilesAt(uint8_t row, uint8_t col, std::pair<int8_t, int8_t> increaser) const;

    BoardState _boardTiles;

    /* This acts a proxy class to expose a double subscription syntax for CBoards without exposing all the inner data
     * structure methods so you can do things like:
     *
     * CBoard b;
     * b[3][4] = ETileType.Empty
     *
     * But not things like this:
     * auto ref = b[3];
     * ref.push_back(4); //Error
     *
     */
    class BoardIndexer
    {
        friend class CBoard;

    public:

        TileType& operator[](std::size_t idx)
        {
            return _rowRef->operator[](idx);
        }

        const TileType& operator[](std::size_t idx) const
        {
            return _rowRef->operator[](idx);
        }

    private:

        BoardIndexer(){};

        void SetRow(std::vector<TileType> *row)
        {
            _rowRef = row;
        }



    private:
        std::vector<TileType> *_rowRef;
    };

    CBoard::BoardIndexer _boardIndexer;

public:

    // Override the operator after the definition of BoardIndexer
    BoardIndexer& operator[](std::size_t idx);
};

#endif /* CBoard_hpp */
