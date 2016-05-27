//
//  GenericBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "GenericBoard.hpp"

std::vector<TilePosition> MergeVectors(std::vector<TilePosition> dest, std::vector< std::vector<TilePosition> > srcList)
{
    for( auto src : srcList )
    {
        dest.insert(
                    dest.end(),
                    std::make_move_iterator(src.begin()),
                    std::make_move_iterator(src.end())
                    );
    }

    return dest;
}

GenericBoard::GenericBoard(BoardState boardState):
    _boardTiles(boardState),
    kRowIncreaser(std::make_pair<int8_t, int8_t>(0,1)),
    kColIncreaser(std::make_pair<int8_t, int8_t>(1,0)),
    kMainDiagIncreaser(std::make_pair<int8_t, int8_t>(1,1)),
    kSecDiagIncreaser(std::make_pair<int8_t, int8_t>(-1,1))
{

}

GenericBoard::GenericBoard(uint8_t rows, uint8_t columns, TileType initialValue):
    _boardTiles(rows, std::vector<TileType>(columns, initialValue))
{

}

GenericBoard::BoardIndexer& GenericBoard::operator[](std::size_t idx)
{
    _boardIndexer.SetRow(&_boardTiles[idx]);
    return _boardIndexer;
}

std::vector<TilePosition> GenericBoard::GetAllAdjacentTiles(uint8_t row, uint8_t col) const
{
    std::vector<TilePosition> result = MergeVectors(SearchAdjacentTilesAt(row, col, kRowIncreaser),
                                                    {
                                                        SearchAdjacentTilesAt(row, col, kColIncreaser),
                                                        SearchAdjacentTilesAt(row, col, kMainDiagIncreaser),
                                                        SearchAdjacentTilesAt(row, col, kSecDiagIncreaser),
                                                    });

    result.push_back(TilePosition(row, col));

    return result;
}

std::vector<TilePosition> GenericBoard::GetRowAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kRowIncreaser);
    result.push_back(TilePosition(row, col));

    return result;
}

std::vector<TilePosition> GenericBoard::GetColAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kColIncreaser);
    result.push_back(TilePosition(row, col));

    return result;
}

std::vector<TilePosition> GenericBoard::GetRowAndColAdjacentTiles(uint8_t row, uint8_t col) const
{
    std::vector<TilePosition> result = MergeVectors(SearchAdjacentTilesAt(row, col, kRowIncreaser),
                                                    {SearchAdjacentTilesAt(row, col, kColIncreaser)});

    result.push_back(TilePosition(row, col));

    return result;
}

std::vector<TilePosition> GenericBoard::GetMainDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kMainDiagIncreaser);
    result.push_back(TilePosition(row, col));

    return result;
}

std::vector<TilePosition> GenericBoard::GetSecondaryDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, kSecDiagIncreaser);
    result.push_back(TilePosition(row, col));

    return result;
}


std::vector<TilePosition> GenericBoard::GetDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    std::vector<TilePosition> result = MergeVectors(SearchAdjacentTilesAt(row, col, kMainDiagIncreaser),
                                                    {SearchAdjacentTilesAt(row, col, kSecDiagIncreaser)});

    result.push_back(TilePosition(row, col));

    return result;
}

std::vector<TilePosition> GenericBoard::SearchAdjacentTilesAt(uint8_t row, uint8_t col, std::pair<int8_t,int8_t> increaser) const
{
    std::vector<TilePosition> result;

    TileType matchTileValue = _boardTiles[row][col];

    int rowItr = row, colItr = col;

    rowItr += increaser.first;
    colItr += increaser.second;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && _boardTiles[rowItr][colItr] == matchTileValue)
    {
        result.push_back(TilePosition(rowItr, colItr));

        rowItr += increaser.first;
        colItr += increaser.second;
    }

    rowItr = row, colItr = col;
    rowItr -= increaser.first;
    colItr -= increaser.second;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && _boardTiles[rowItr][colItr] == matchTileValue)
    {
        result.push_back(TilePosition(rowItr, colItr));

        rowItr -= increaser.first;
        colItr -= increaser.second;
    }

    return result;
}
