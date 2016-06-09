//
//  CBoard.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef GenericBoard_hpp
#define GenericBoard_hpp

#include <vector>
#include <unordered_set>


enum ESpecialBoardPieces {
    Disabled = -1,
    Empty = 0,
};

typedef short TileType;
typedef short TileCoordinate;

struct TilePosition
{
    TilePosition():TilePosition(0,0){};
    TilePosition(TileCoordinate Row, TileCoordinate Col):row(Row),col(Col){};
    TileCoordinate row;
    TileCoordinate col;
    
    bool operator==(const TilePosition& other) const
    {
        return row == other.row && col == other.col;
    }
    
    bool operator!=(const TilePosition &other)
    {
        return !this->operator==(other);
    }

};


// Data structures to query and change the board state
typedef std::unordered_set<TilePosition> TilesSet;
struct TileMovement
{
    TilePosition from;
    TilePosition to;
    
    TileMovement(TilePosition From, TilePosition To): from(From), to(To) {}
    
    
    bool operator==(const TileMovement &other) const {
        return this->from == other.from && this->to == other.to;
    }
    
    
    bool operator!=(const TileMovement &other)
    {
        return !this->operator==(other);
    }
};

typedef std::vector<TileMovement> TilesMovementSet;

// Need to specializy the std::hash template to allow hashing of TilePosition in order to
// use them in data structures requiring a hash (map, sets, etc)
namespace std {
    template <> struct hash<TilePosition>
    {
        size_t operator()(const TilePosition & pos) const
        {
            return (51 + std::hash<TileCoordinate>()(pos.row)) * 51 + std::hash<TileCoordinate>()(pos.col);
        }
    };
}

typedef std::vector< std::vector<TileType> > BoardState;

/*
 * Represents a generic game NxM board
 *
 * Allows getting and setting different tile values (int8_t types) in an agnostic way
 * Implements operations to search for adjacent tiles that have the same values
 * 
 */
class GenericBoard
{

public:

    typedef std::function<bool(std::unordered_set<TilePosition>)> FilterFunc;
    
    // Allows using initialization_lists to set up a board, usefull for testing purposes
    // TODO: check if this should be changed to a Mememto pattern
    explicit GenericBoard(BoardState boardData);

    // Sets up an board of a given rows and columns with the Empty tile value
    explicit GenericBoard(TileCoordinate rows, TileCoordinate columns);

    const BoardState& boardState() { return mBoardTiles; };
    
    void ResetBoardState();
    void ResetBoardState(TileCoordinate rows, TileCoordinate columns);
    
    
    void boardState(const BoardState& newState ) { mBoardTiles = newState; };
    
    TileCoordinate rows() const { return mBoardTiles.size(); }
    TileCoordinate columns() const { return mBoardTiles[0].size(); }
    
    
    bool IsPositionInsideBoardBounds(const TilePosition &IsPositionInsideBoardBoundspos) const;
    void RemovePieces(const TilesSet &pieces);
    void MovePieces(const TilesMovementSet &pieces);
    
    std::unordered_set<TilePosition> GetRowAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter = nullptr) const;
    std::unordered_set<TilePosition> GetColAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter = nullptr) const;
    std::unordered_set<TilePosition> GetRowAndColAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter = nullptr) const;
    std::unordered_set<TilePosition> GetMainDiagonalAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter = nullptr) const;
    std::unordered_set<TilePosition> GetSecondaryDiagonalAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter = nullptr) const;
    std::unordered_set<TilePosition> GetDiagonalAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter = nullptr) const;
    std::unordered_set<TilePosition> GetAllAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc func = nullptr) const;
    
protected:
    /**
     * Current state of the board
     */
    BoardState mBoardTiles;

    
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
    std::unordered_set<TilePosition> SearchAdjacentTilesAt(const BoardState &boardTiles,
                                                           TileCoordinate row,
                                                           TileCoordinate col,
                                                           const std::pair<int8_t,int8_t> &increaser,
                                                           FilterFunc filter) const;
private:
    
    const std::pair<TileCoordinate, TileCoordinate> kRowIncreaser = std::make_pair<TileCoordinate, TileCoordinate>(0,1);
    const std::pair<TileCoordinate, TileCoordinate> kColIncreaser = std::make_pair<TileCoordinate, TileCoordinate>(1,0);
    const std::pair<TileCoordinate, TileCoordinate> kMainDiagIncreaser = std::make_pair<TileCoordinate, TileCoordinate>(1,1);
    const std::pair<TileCoordinate, TileCoordinate> kSecDiagIncreaser = std::make_pair<TileCoordinate, TileCoordinate>(-1,1);
    

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
        friend class GenericBoard;

    public:

        TileType& operator[](std::size_t idx)
        {
            return pRowRef->operator[](idx);
        }

        const TileType& operator[](std::size_t idx) const
        {
            return pRowRef->operator[](idx);
        }

    private:

        BoardIndexer(){};

        void SetRow(std::vector<TileType> *row)
        {
            pRowRef = row;
        }



    private:
        std::vector<TileType> *pRowRef;
    };

    GenericBoard::BoardIndexer mBoardIndexer;

public:

    // Override the operator after the definition of BoardIndexer
    BoardIndexer& operator[](std::size_t idx);
    const BoardIndexer& operator[](std::size_t idx) const;
};

#endif /* GenericBoard_hpp */
