//
//  GenericBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "GenericBoard.hpp"

std::ostream& operator<<(std::ostream& os, const TilePosition& pos)
{
    os << "[" << pos.row << ", " << pos.col << "]";
    return os;
}




std::unordered_set<TilePosition> Merge(std::initializer_list< std::unordered_set<TilePosition> > srcList)
{
    std::unordered_set<TilePosition> dest;
    
    for( auto src : srcList )
    {
        dest.insert(src.begin(), src.end());
    }
    
    return dest;
}

GenericBoard::GenericBoard(BoardState boardState):
    mBoardTiles(boardState),
    kRowIncreaser(std::make_pair<int8_t, int8_t>(0,1)),
    kColIncreaser(std::make_pair<int8_t, int8_t>(1,0)),
    kMainDiagIncreaser(std::make_pair<int8_t, int8_t>(1,1)),
    kSecDiagIncreaser(std::make_pair<int8_t, int8_t>(-1,1))
{

}

GenericBoard::GenericBoard(uint8_t rows, uint8_t columns, TileType initialValue):
    mBoardTiles(rows, std::vector<TileType>(columns, initialValue))
{

}


bool GenericBoard::IsPositionInsideBoardBounds(const TilePosition &pos) const
{
    return pos.row >= 0 && pos.row < rows() && pos.col >= 0 && pos.col < columns();
}


void GenericBoard::RemovePieces(const TilesSet &pieces)
{
    for(auto piece: pieces)
    {
        mBoardTiles[piece.row][piece.col] = ESpecialBoardPieces::Empty;
    }
}

void GenericBoard::MovePieces(const TilesMovementSet &pieces)
{
    
    for(int i = 0; i < pieces.size(); ++i)
    {
        TileMovement move = pieces[i];
        TileType valueToMove = mBoardTiles[move.from.row][move.from.col];
        mBoardTiles[move.from.row][move.from.col] = ESpecialBoardPieces::Empty;
        mBoardTiles[move.to.row][move.to.col] = valueToMove;
    }
    
}

GenericBoard::BoardIndexer& GenericBoard::operator[](std::size_t idx)
{
    mBoardIndexer.SetRow(&mBoardTiles[idx]);
    return mBoardIndexer;
}

std::unordered_set<TilePosition> GenericBoard::GetAllAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result =  Merge({
        SearchAdjacentTilesAt(row, col, kRowIncreaser),
        SearchAdjacentTilesAt(row, col, kColIncreaser),
        SearchAdjacentTilesAt(row, col, kMainDiagIncreaser),
        SearchAdjacentTilesAt(row, col, kSecDiagIncreaser)
    });
    
    result.insert(TilePosition(row,col));
    
    return result;

}

std::unordered_set<TilePosition> GenericBoard::GetRowAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kRowIncreaser);
    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetColAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kColIncreaser);
    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetRowAndColAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = Merge({SearchAdjacentTilesAt(row, col, kRowIncreaser),
                         SearchAdjacentTilesAt(row, col, kColIncreaser)});

    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetMainDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kMainDiagIncreaser);
    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetSecondaryDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kSecDiagIncreaser);
    result.insert(TilePosition(row, col));

    return result;
}


std::unordered_set<TilePosition> GenericBoard::GetDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = Merge({SearchAdjacentTilesAt(row, col, kMainDiagIncreaser),
                                              SearchAdjacentTilesAt(row, col, kSecDiagIncreaser)});

    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::SearchAdjacentTilesAt(uint8_t row, uint8_t col, std::pair<int8_t,int8_t> increaser) const
{
    std::unordered_set<TilePosition> result;

    TileType matchTileValue = mBoardTiles[row][col];

    int rowItr = row, colItr = col;

    rowItr += increaser.first;
    colItr += increaser.second;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && mBoardTiles[rowItr][colItr] == matchTileValue)
    {
        result.insert(TilePosition(rowItr, colItr));

        rowItr += increaser.first;
        colItr += increaser.second;
    }

    rowItr = row, colItr = col;
    rowItr -= increaser.first;
    colItr -= increaser.second;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && mBoardTiles[rowItr][colItr] == matchTileValue)
    {
        result.insert(TilePosition(rowItr, colItr));

        rowItr -= increaser.first;
        colItr -= increaser.second;
    }

    return result;
}
