//
//  PlayerBlockMovement.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 11/6/16.
//
//

#include "catch.hpp"
#include "ColumnsBoard.hpp"
#include "PlayerBlock.hpp"

TEST_CASE( "PlayerBlock", "[GameModel]" ) {

    SECTION("Can move the block pieces in cycles")
    {
        PlayerBlock block;
        
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

    SECTION("Can move player to the right") {
        ColumnsBoard board({
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,1},
            {0,0,0,1,1},
            {0,0,1,1,1},
            {0,1,1,1,1}
        }, 0);
        
        PlayerBlock block;
        block.SetNewPieces({1,2,3});
        
        REQUIRE(board.CanMovePlayerBlockRight(block));
        
        block.position().col = 3;
        
        REQUIRE_FALSE(board.CanMovePlayerBlockRight(block));
        
        block.position().col = 2;
        block.position().row = 0;
        
        REQUIRE(board.CanMovePlayerBlockRight(block));
        
        block.position().row = 0.5;
        REQUIRE_FALSE(board.CanMovePlayerBlockRight(block));
    }
    
    
    SECTION("Can move player to the left") {
        ColumnsBoard board({
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,1},
            {0,0,0,1,1},
            {0,0,1,1,1},
            {0,1,1,1,1}
        }, 0);
        
        PlayerBlock block;
        block.SetNewPieces({1,2,3});
        
        REQUIRE_FALSE(board.CanMovePlayerBlockLeft(block));
        block.position().col += 1;
        
        REQUIRE(board.CanMovePlayerBlockLeft(block));
    }
    
    SECTION("Can move player down") {
        ColumnsBoard board({
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,1},
            {0,0,0,1,1},
            {0,0,1,1,1},
            {0,1,1,1,1}
        }, 0);
        
        PlayerBlock block;
        block.SetNewPieces({1,2,3});
        
        REQUIRE(board.CanMovePlayerBlockDown(block));
        
        block.position().col = 3;
        
        REQUIRE_FALSE(board.CanMovePlayerBlockDown(block));
    }

    SECTION("Can move player in middle of a tile with collision") {
        ColumnsBoard board({
            {0,0,0},
            {0,0,0},
            {0,0,1},
            {0,1,1},
            {1,1,1},
            {1,1,1}
        }, 0);
        
        PlayerBlock block;
        block.SetNewPieces({1,2,3});
        block.position(PositionF(0,0));
        REQUIRE(board.CanMovePlayerBlockRight(block));
        
        block.position(PositionF(0.5,0));
        REQUIRE_FALSE(board.CanMovePlayerBlockRight(block));
        
    }
    
    SECTION("Can set player block in invalid position") {
        ColumnsBoard board({
            {0,0,0},
            {0,0,0},
            {0,0,1},
            {0,1,1},
            {1,1,1},
            {1,1,1}
        }, 0);
        
        PlayerBlock block;
        block.SetNewPieces({1,2,3});
        block.position(PositionF(0,2));
        REQUIRE_FALSE(board.CanMovePlayerBlockTo(TileOffset(), block));
    }


}