//
//  ColumnsBoardTest.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 27/5/16.
//
//

#include "catch.hpp"
#include "ColumnsBoard.hpp"

TEST_CASE( "PlayerBlock", "[GameModel]" ) {
    PlayerBlock block;
    
    
    SECTION("Can move the block pieces in cycles")
    {
        block.SetNewPieces({1,2,3});
        
        REQUIRE(block.size() == 3);
        
        REQUIRE(block[0] == 1);
        REQUIRE(block[1] == 2);
        REQUIRE(block[2] == 3);
        
        
        block.MovePieces();
        
        REQUIRE(block[0] == 2);
        REQUIRE(block[1] == 3);
        REQUIRE(block[2] == 1);
        
        block.MovePieces();
        
        REQUIRE(block[0] == 3);
        REQUIRE(block[1] == 1);
        REQUIRE(block[2] == 2);
        
        block.MovePieces();
        
        REQUIRE(block[0] == 1);
        REQUIRE(block[1] == 2);
        REQUIRE(block[2] == 3);
    }
}

TEST_CASE( "ColumnsBoard", "[GameModel]" ) {
    ColumnsBoard board({
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1},
        {0,0,0,1,1},
        {0,0,1,1,1},
        {0,1,1,1,1}
    });
    
    SECTION("Board state updates when position player block") {
        
        board.InitPlayerBlockInitialPosition(TilePosition(0,3));
        
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE(board[0][3] == 2);
        REQUIRE(board[1][3] == 2);
        REQUIRE(board[2][3] == 2);
        
    }
    
    SECTION("Can move player to the right") {
    }
    
    
    SECTION("Can move player to the left") {
    }
    
    SECTION("Can move player down") {
        board.InitPlayerBlockInitialPosition(TilePosition(0,0));
        
        board.ResetPlayerBlock({2,2,2});
        
        
        REQUIRE(board.MovePlayerBlockDown());
        
        REQUIRE(board[0][0] == ESpecialBoardPieces::Empty);
        REQUIRE(board[1][0] == 2);
        REQUIRE(board[2][0] == 2);
        REQUIRE(board[3][0] == 2);
    }
    
    
    SECTION("Can't move player block to occupied tiles to the left") {
        board.InitPlayerBlockInitialPosition(TilePosition(0,0));
        
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE_FALSE(board.MovePlayerBlockLeft());
    }
    
    SECTION("Can't move player block to occupied tiles to the right") {
        
        board.InitPlayerBlockInitialPosition(TilePosition(0,0));
        
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE_FALSE(board.MovePlayerBlockRight());
    }
    
    SECTION("Can't move player block to occupied tiles below it") {
        
        board.InitPlayerBlockInitialPosition(TilePosition(0,0));
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE(board.MovePlayerBlockDown());
        REQUIRE(board.MovePlayerBlockDown());
        REQUIRE(board.MovePlayerBlockDown());
        
        REQUIRE_FALSE(board.MovePlayerBlockDown());        
    }
    
    SECTION("Moving player block updates board state")
    {
        board.InitPlayerBlockInitialPosition(TilePosition(0,0));
        REQUIRE(board.ResetPlayerBlock({2,2,2}));
        
        REQUIRE(board[0][0] == 2);
        REQUIRE(board[1][0] == 2);
        REQUIRE(board[2][0] == 2);
        
        REQUIRE(board.MovePlayerBlockRight());
        
        // Old position
        REQUIRE(board[0][0] == 0);
        REQUIRE(board[1][0] == 0);
        REQUIRE(board[2][0] == 0);
        
        // New position
        REQUIRE(board[0][1] == 2);
        REQUIRE(board[1][1] == 2);
        REQUIRE(board[2][1] == 2);
        
    }
    
    SECTION("Moving pieces inside player block updates board state")
    {
        board.InitPlayerBlockInitialPosition(TilePosition(0,0));
        board.ResetPlayerBlock({2,3,4});
        
        REQUIRE(board[0][0] == 2);
        REQUIRE(board[1][0] == 3);
        REQUIRE(board[2][0] == 4);
        
        board.MovePlayerBlockPieces();
        
    
        REQUIRE(board[0][0] == 3);
        REQUIRE(board[1][0] == 4);
        REQUIRE(board[2][0] == 2);
        
    }
}

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


TEST_CASE("Change board state") {
    
    SECTION("Destroy pieces") {
        

        // Start with a board that does not have any pieces to destroy
        ColumnsBoard board({
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,2},
            {0,0,0,1,1},
            {0,0,1,2,1},
            {0,2,1,2,2}
        });
        
        board.InitNumEqualPiecesToDestroy(3);
        
        // supose we get a player block in board position [1,2] and the block contains the pieces [2,2,1]
        board.InitPlayerBlockInitialPosition(TilePosition(1,2));
        board.ResetPlayerBlock({2,2,1});
        
        // The new boardState should be like this
        BoardState newBoardState = {
            {0,0,0,0,0},
            {0,0,2,0,0},
            {0,0,2,0,2},
            {0,0,1,1,1},
            {0,0,1,2,1},
            {0,2,1,2,2}
        };
       
        REQUIRE(CompareBoardStates(newBoardState, board.boardState()));
        
        // Now we can search for adjacent pieces, but we only need to seach in the player block positions
        // as it was the only parts that changed
        auto pieces = board.FindPiecesToDestroy();
        
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
        });

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
    
    SECTION("Game end condition is computed correctly")
    {
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
        });
        
        board.InitNumRowsGameOver(3);
        
        REQUIRE_FALSE(board.IsGameOverConditionFullfilled());
        
        //at position (0,0) by default
        board.ResetPlayerBlock({1,1,2});
        
        REQUIRE(board.IsGameOverConditionFullfilled());
    }
    
    SECTION("Can't reset the state of the player block if the board is not empty") {
        ColumnsBoard board( {
            {0,0,0,0,0},
            {0,0,0,0,2},
            {0,0,0,0,1},
            {0,0,3,2,1},
            {0,2,1,2,2}
        });
        
        board.InitPlayerBlockInitialPosition({0,2});
        
        REQUIRE(board.ResetPlayerBlock({1,2,3}));
        
        REQUIRE_FALSE(board.ResetPlayerBlock({1,2,3}));

    }
}



