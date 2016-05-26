//
//  CBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "CBoard.hpp"

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

CBoard::CBoard(std::vector< std::vector<TileType> > boardData):
    _boardTiles(boardData)
{

}

CBoard::CBoard(uint8_t rows, uint8_t columns, TileType initialValue):
    _boardTiles(rows, std::vector<TileType>(columns, initialValue))
{

}

CBoard::BoardIndexer& CBoard::operator[](std::size_t idx)
{
    _boardIndexer.SetRow(&_boardTiles[idx]);
    return _boardIndexer;
}

std::vector<TilePosition> CBoard::GetAllAdjacentTiles(uint8_t row, uint8_t col) const
{
    std::vector<TilePosition> result = MergeVectors(SearchAdjacentTilesAt(row, col, MakeTilePos(0,1)),
                                                    {
                                                        SearchAdjacentTilesAt(row, col, MakeTilePos(1,0)),
                                                        SearchAdjacentTilesAt(row, col, MakeTilePos(1,1)),
                                                        SearchAdjacentTilesAt(row, col, MakeTilePos(-1,1)),
                                                    });

    result.push_back(std::make_pair(row, col));

    return result;
}

std::vector<TilePosition> CBoard::GetRowAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, MakeTilePos(0,1));
    result.push_back(std::make_pair(row, col));

    return result;
}

std::vector<TilePosition> CBoard::GetColAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, MakeTilePos(1,0));
    result.push_back(std::make_pair(row, col));

    return result;
}

std::vector<TilePosition> CBoard::GetRowAndColAdjacentTiles(uint8_t row, uint8_t col) const
{
    std::vector<TilePosition> result = MergeVectors(SearchAdjacentTilesAt(row, col, MakeTilePos(0,1)),
                                                    {SearchAdjacentTilesAt(row, col, MakeTilePos(1,0))});

    result.push_back(std::make_pair(row, col));

    return result;
}

std::vector<TilePosition> CBoard::GetMainDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, MakeTilePos(1,1));
    result.push_back(std::make_pair(row, col));

    return result;
}

std::vector<TilePosition> CBoard::GetSecondaryDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    auto result = SearchAdjacentTilesAt(row, col, MakeTilePos(1,-1));
    result.push_back(std::make_pair(row, col));

    return result;
}


std::vector<TilePosition> CBoard::GetDiagonalAdjacentTiles(uint8_t row, uint8_t col) const
{
    std::vector<TilePosition> result = MergeVectors(SearchAdjacentTilesAt(row, col, MakeTilePos(1,1)),
                                                    {SearchAdjacentTilesAt(row, col, MakeTilePos(-1,1))});

    result.push_back(std::make_pair(row, col));

    return result;
}

std::vector<TilePosition> CBoard::SearchAdjacentTilesAt(uint8_t row, uint8_t col, std::pair<int8_t, int8_t> increaser) const
{
    std::vector<TilePosition> result;

    TileType matchTileValue = _boardTiles[row][col];

    int rowItr = row, colItr = col;

    rowItr += increaser.first;
    colItr += increaser.second;
    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && _boardTiles[rowItr][colItr] == matchTileValue)
    {
        result.push_back(std::make_pair(rowItr, colItr));

        rowItr += increaser.first;
        colItr += increaser.second;
    }

    rowItr = row, colItr = col;
    rowItr -= increaser.first;
    colItr -= increaser.second;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && _boardTiles[rowItr][colItr] == matchTileValue)
    {
        result.push_back(std::make_pair(rowItr, colItr));

        rowItr -= increaser.first;
        colItr -= increaser.second;
    }

    return result;
}
