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
    
    SECTION("Is safe to iterate without pieces") {
        for(auto i = 0; i < block.size(); ++i)
        {
        
        }
    }
    
    
    SECTION("Can move the block pieces in cycles")
    {
        block.SetNewPieces({1,2,3});
        
//        CBoard b(3, 4, 11);
//        
//        REQUIRE(b.rows() == 3);
//        REQUIRE(b.columns() == 4);
//        REQUIRE(b[0][0] == 11);
//        REQUIRE(b[2][3] == 11);
    }
}