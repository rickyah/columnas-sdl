//
//  ColumnsBoardTest.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#include "catch.hpp"
#include "ColumnsBoard.hpp"

bool CompareBoardStates(const BoardState &first, const BoardState &second)
{
    for(int i = 0; i < first.size(); ++i)
    {
        for(int j = 0; j < first[i].size(); ++j)
        {
            if (first[i][j] != second[i][j])
            {
                return false;
            }
        }
    }
    
    return true;
}


TEST_CASE("Change board state", "[GameModel]") {
    
    SECTION("Destroy pieces") {
        

        // Suppose we start with a board that does not have any pieces to destroy
        //    {0,0,0,0,0}
        //    {0,0,0,0,0}
        //    {0,0,0,0,2}
        //    {0,0,0,1,1}
        //    {0,0,1,2,1}
        //    {0,2,1,2,2}
        //
        // and we get a player block in board position [1,2] and the block contains the pieces [2,2,1]
        // The new boardState should be like this
        ColumnsBoard board({
            {0,0,0,0,0},
            {0,0,2,0,0},
            {0,0,2,0,2},
            {0,0,1,1,1},
            {0,0,1,2,1},
            {0,2,1,2,2}
        }, 0, 3);
        
        board.numEqualPiecesToDestroy(3);
        
        // Now we can search for adjacent pieces, but we only need to seach in the player block positions
        // as it was the only parts that changed
        auto pieces = board.FindPiecesToDestroy({{1,2}, {2,2}, {3,2}});
        
        REQUIRE(pieces.size() == 5);
        
        // Destroy the pieces
        board.RemovePieces(pieces);
        
        
        // And the board should be like this
        BoardState newBoardState2 = {
            {0,0,0,0,0},
            {0,0,2,0,0},
            {0,0,2,0,2},
            {0,0,0,0,0},
            {0,0,0,2,1},
            {0,2,0,2,2}
        };
        
        REQUIRE(CompareBoardStates(newBoardState2, board.boardState()));
    }
    
    SECTION("Find pieces to fall")
    {
        ColumnsBoard board( {   // Destroyed positions:
            {0,0,0,0,0},        // {0,0,0,0,0}
            {0,0,2,0,0},        // {0,0,2,0,0}
            {0,0,2,0,2},        // {0,0,2,0,2}
            {2,0,0,0,0},        // {2,0,*,*,*}
            {0,0,0,2,1},        // {*,*,*,2,1}
            {2,2,0,2,2}         // {2,2,*,2,2}
        }, 0);

        auto piecesToFall = board.FindPiecesToMoveInSubset({
            {3,2},{3,3},{3,4},
            {4,0},{4,1},{4,2},
            {5,2}
        });
        
        REQUIRE(piecesToFall.size() == 4);
        

        // Column 0: 1 piece
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({3,0},{4,0})) != piecesToFall.end());
        
        // Column 1: 0 pieces
        
        // Column 2: 2 pieces
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({2,2},{5,2})) != piecesToFall.end());
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({1,2},{4,2})) != piecesToFall.end());
        
        // Column 3: 0 pieces
        
        // Column 4: 1 pieces
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({2,4},{3,4})) != piecesToFall.end());
    }
    
    SECTION("Find Pieces to fall") {
        ColumnsBoard board( {
            {0,0,0,0,0},        // {0,0,0,0,0}
            {0,0,0,0,0},        // {0,0,0,0,0}
            {0,0,0,0,0},        // {0,0,0,0,2}
            {0,0,0,2,4},        // {0,0,0,3,4}
            {0,0,3,2,1},        // {0,0,3,2,1}
            {0,0,0,0,0}         // {0,0,*,*,*}
        }, 0);
        
        auto piecesToFall = board.FindPiecesToMoveInSubset({
            {5,2}, {5,3}, {5,4}
        });
        
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({4,2},{5,2})) != piecesToFall.end());
        
        
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({4,3},{5,3})) != piecesToFall.end());
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({3,3},{4,3})) != piecesToFall.end());
        
        
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({4,4},{5,4})) != piecesToFall.end());
        REQUIRE(std::find(piecesToFall.begin(), piecesToFall.end(), TileMovement({3,4},{4,4})) != piecesToFall.end());
    }
    
    SECTION("Game end condition is computed correctly") {
        ColumnsBoard board( {
            
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            // board game, any piece above this row should end the game
            {0,0,2,0,0},
            {0,0,2,0,2},
            {0,0,3,0,1},
            {0,0,3,2,1},
            {0,2,1,2,2}
        }, 3);
        
        
        REQUIRE_FALSE(board.IsGameOverConditionFullfilled());
        
        board[2][3] = 1;
        board[3][3] = 1;
        board[4][3] = 2;
        
        REQUIRE(board.IsGameOverConditionFullfilled());
    }
 
    SECTION("Multistreaks") {
        ColumnsBoard board( {

            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,0,3,0,0},
            {0,0,3,0,2},
            {0,0,3,0,1},
            {0,0,1,2,1},
            {0,2,1,2,2}
        }, 3);
        
        
        auto pieces = board.FindPiecesToDestroy({ TilePosition(3, 2) });
        board.RemovePieces(pieces);
        TilesMovementSet piecesToMove = board.FindAllPiecesToMove();
        board.MovePieces( piecesToMove);
        
        BoardState newState ={
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,2},
            {0,0,1,0,1},
            {0,0,1,2,1},
            {0,2,1,2,2}
        };

        REQUIRE(CompareBoardStates(board.boardState(), newState));
        
        pieces.clear();

        std::transform(piecesToMove.begin(), piecesToMove.end(), std::inserter(pieces, pieces.begin()), [](const TileMovement &tileToMove) {
            return tileToMove.to;
        });
        
        board.RemovePieces(board.FindPiecesToDestroy(pieces));
        board.MovePieces( board.FindAllPiecesToMove());
        
        newState = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,2},
            {0,0,0,0,1},
            {0,0,0,2,1},
            {0,2,0,2,2}
        };
        
        REQUIRE(CompareBoardStates(board.boardState(), newState));
        
    }
    
}
