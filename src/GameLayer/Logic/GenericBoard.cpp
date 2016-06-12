//
//  GenericBoard.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#include "GenericBoard.hpp"

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
    mBoardTiles(boardState)
{

}

GenericBoard::GenericBoard(TileCoordinate rows, TileCoordinate columns):
    mBoardTiles(rows, std::vector<TileType>(columns, ESpecialBoardPieces::Empty))
{

}


bool GenericBoard::IsPositionInsideBoardBounds(const TilePosition &pos) const
{
    return pos.row >= 0 && pos.row < rows() && pos.col >= 0 && pos.col < columns();
}


void GenericBoard::RemovePieces(const TilesSet &pieces)
{
    for(auto &piece: pieces)
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
    mBoardIndexer.SetRow(mBoardTiles[idx]);
    return mBoardIndexer;
}

const GenericBoard::BoardIndexer& GenericBoard::operator[](std::size_t idx) const
{
    mBoardIndexer.SetRow(mBoardTiles[idx]);
    return mBoardIndexer;
}

std::unordered_set<TilePosition> GenericBoard::GetAllAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result =  Merge(
    {
        SearchAdjacentTilesAt(mBoardTiles, row, col, kRowIncreaser, filter),
        SearchAdjacentTilesAt(mBoardTiles, row, col, kColIncreaser, filter),
        SearchAdjacentTilesAt(mBoardTiles, row, col, kMainDiagIncreaser, filter),
        SearchAdjacentTilesAt(mBoardTiles, row, col, kSecDiagIncreaser, filter)
    });
    
    result.insert(TilePosition(row,col));
    
    return result;

}

std::unordered_set<TilePosition> GenericBoard::GetRowAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result = SearchAdjacentTilesAt(mBoardTiles, row, col, kRowIncreaser, filter);
    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetColAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result = SearchAdjacentTilesAt(mBoardTiles, row, col, kColIncreaser, filter);
    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetRowAndColAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result = Merge(
    {
        SearchAdjacentTilesAt(mBoardTiles, row, col, kRowIncreaser, filter),
        SearchAdjacentTilesAt(mBoardTiles, row, col, kColIncreaser,filter)
    });

    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetMainDiagonalAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result = SearchAdjacentTilesAt(mBoardTiles, row, col, kMainDiagIncreaser, filter);
    result.insert(TilePosition(row, col));

    return result;
}

std::unordered_set<TilePosition> GenericBoard::GetSecondaryDiagonalAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result = SearchAdjacentTilesAt(mBoardTiles, row, col, kSecDiagIncreaser, filter);
    result.insert(TilePosition(row, col));

    return result;
}


std::unordered_set<TilePosition> GenericBoard::GetDiagonalAdjacentTiles(TileCoordinate row, TileCoordinate col, FilterFunc filter) const
{
    auto result = Merge(
    {
        SearchAdjacentTilesAt(mBoardTiles, row, col, kMainDiagIncreaser, filter),
        SearchAdjacentTilesAt(mBoardTiles, row, col, kSecDiagIncreaser, filter)
    });

    result.insert(TilePosition(row, col));

    return result;
}

void GenericBoard::ResetBoardState()
{
    for(auto &row: mBoardTiles )
    {
        std::fill(row.begin(), row.end(), ESpecialBoardPieces::Empty);
    }
}

void GenericBoard::ResetBoardState(TileCoordinate rows, TileCoordinate columns)
{
    mBoardTiles.resize(rows);

    for(auto &row: mBoardTiles )
    {
        row.resize(columns, ESpecialBoardPieces::Empty);
    }
    
}

std::unordered_set<TilePosition> GenericBoard::SearchAdjacentTilesAt(const BoardState& boardTiles,
                                                                     TileCoordinate row,
                                                                     TileCoordinate col,
                                                                     const TileOffset &increaser,
                                                                     FilterFunc filter ) const
{
    std::unordered_set<TilePosition> result;

    TileType matchTileValue = boardTiles[row][col];

    if (matchTileValue <= ESpecialBoardPieces::Empty)
    {
        return result;
    }
    
    int rowItr = row, colItr = col;

    rowItr += increaser.rowOffset;
    colItr += increaser.colOffset;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && mBoardTiles[rowItr][colItr] == matchTileValue)
    {
        result.insert(TilePosition(rowItr, colItr));

        rowItr += increaser.rowOffset;
        colItr += increaser.colOffset;
    }

    rowItr = row, colItr = col;
    rowItr -= increaser.rowOffset;
    colItr -= increaser.colOffset;

    while(rowItr >= 0 && colItr >= 0 && rowItr < rows() && colItr < columns() && mBoardTiles[rowItr][colItr] == matchTileValue)
    {
        result.insert(TilePosition(rowItr, colItr));

        rowItr -= increaser.rowOffset;
        colItr -= increaser.colOffset;
    }
    
    if ( filter && filter(result) )
    {
        result.clear();
    }
    
    return result;
}
