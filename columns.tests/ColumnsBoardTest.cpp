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
        
        board.SetPlayerBlockInitialPosition(TilePosition(0,3));
        
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
        board.SetPlayerBlockInitialPosition(TilePosition(0,0));
        
        board.ResetPlayerBlock({2,2,2});
        
        
        REQUIRE(board.MovePlayerBlockDown());
        
        REQUIRE(board[0][0] == ESpecialBoardPieces::Empty);
        REQUIRE(board[1][0] == 2);
        REQUIRE(board[2][0] == 2);
        REQUIRE(board[3][0] == 2);
    }
    
    
    SECTION("Can't move player block to occupied tiles") {
        board.SetPlayerBlockInitialPosition(TilePosition(0,0));
        
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE_FALSE(board.MovePlayerBlockLeft());
        
        
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE_FALSE(board.MovePlayerBlockRight());
        
        
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE(board.MovePlayerBlockDown());
        REQUIRE(board.MovePlayerBlockDown());
        REQUIRE(board.MovePlayerBlockDown());
        
        REQUIRE_FALSE(board.MovePlayerBlockDown());
      
        board.ResetPlayerBlock({2,2,2});
        
        REQUIRE(board.MovePlayerBlockDown());
        REQUIRE(board.MovePlayerBlockDown());
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE(board.MovePlayerBlockLeft());
        REQUIRE(board.MovePlayerBlockRight());
        REQUIRE_FALSE(board.MovePlayerBlockRight());
        
    }
    
    SECTION("Moving player block updates board state")
    {
        board.SetPlayerBlockInitialPosition(TilePosition(0,0));
        board.ResetPlayerBlock({2,2,2});
        
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
        board.SetPlayerBlockInitialPosition(TilePosition(0,0));
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