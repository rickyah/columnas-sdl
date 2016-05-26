//
//  columns.tests
//
//  Created by Ricardo Amores Hernández on 26/5/16.

#include "catch.hpp"
#include "CBoard.hpp"


TEST_CASE( "Game Board", "[GameModel]" ) {
    CBoard board({
        {0,0,0,1,1,1},
        {2,0,0,1,1,2},
        {1,0,1,1,1,3},
        {2,2,2,1,1,2},
    });

    REQUIRE(board.rows() == 4);
    REQUIRE(board.columns() == 6);

    SECTION("Init a board with a given initial value")
    {
        CBoard b(3, 4, 11);

        REQUIRE(b.rows() == 3);
        REQUIRE(b.columns() == 4);
        REQUIRE(b[0][0] == 11);
        REQUIRE(b[2][3] == 11);
    }

    SECTION("You can get and set the values of a board using the subscript operator") {
        REQUIRE(board[2][3] == 1);

        board[2][3] = 10;

        REQUIRE(board[2][3] == 10);
    }

    SECTION("Get equal adjacent tiles in a row from a given position") {
        std::vector<TilePosition> result = board.GetRowAdjacentTiles(2, 3);

        REQUIRE(result.size() == 3);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,2)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,4)) != result.end());
    }

    SECTION("Get equal adjacent tiles in a column from a given position") {
        std::vector<TilePosition> result = board.GetColAdjacentTiles(2, 3);

        REQUIRE(result.size() == 4);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(0,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(3,3)) != result.end());
    }

    SECTION("Get equal adjacent tiles in the row and column from a given position") {
        std::vector<TilePosition> result = board.GetRowAndColAdjacentTiles(2, 3);

        REQUIRE(result.size() == 6);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,2)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,4)) != result.end());

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(0,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(3,3)) != result.end());
    }

    SECTION("Get equal adjacent tiles in the main diagonal from a given position") {
        std::vector<TilePosition> result = board.GetMainDiagonalAdjacentTiles(2, 3);

        REQUIRE(result.size() == 2);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(3,4)) != result.end());
    }

    SECTION("Get equal adjacent tiles in the secondary diagonal from a given position") {
        std::vector<TilePosition> result = board.GetSecondaryDiagonalAdjacentTiles(2, 3);

        REQUIRE(result.size() == 3);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(1,4)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(0,5)) != result.end());
    }

    SECTION("Get equal adjacent tiles in both diagonals from a given position") {
        std::vector<TilePosition> result = board.GetDiagonalAdjacentTiles(2, 3);

        REQUIRE(result.size() == 4);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(1,4)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(0,5)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(3,4)) != result.end());
    }


    SECTION("Get alladjacent tiles in rows, columns and diagonals from a given position") {
        std::vector<TilePosition> result = board.GetAllAdjacentTiles(2, 3);

        REQUIRE(result.size() == 9);

        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());

        // rows
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,2)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,4)) != result.end());

        // cols
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(0,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(2,3)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(3,3)) != result.end());

        // Diagonals
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(1,4)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(0,5)) != result.end());
        REQUIRE(std::find(result.begin(), result.end(), CBoard::MakeTilePos(3,4)) != result.end());
    }

}