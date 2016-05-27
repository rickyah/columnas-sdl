////
////  PlayerBlockTests.cpp
////  Columns
////
////  Created by Ricardo Amores Hernández on 27/5/16.
////
////
//
//#include "catch.hpp"
//#include <ColumnsBoard.hpp>
//
//TEST_CASE( "Game Board", "[GameModel]" ) {
//    CBoard board({
//        {0,0,0,1,1,1},
//        {2,0,0,1,1,2},
//        {1,0,1,1,1,3},
//        {2,2,2,1,1,2},
//    });
//    
//    REQUIRE(board.rows() == 4);
//    REQUIRE(board.columns() == 6);
//    
//    SECTION("Init a board with a given initial value")
//    {
//        CBoard b(3, 4, 11);
//        
//        REQUIRE(b.rows() == 3);
//        REQUIRE(b.columns() == 4);
//        REQUIRE(b[0][0] == 11);
//        REQUIRE(b[2][3] == 11);
//    }
//}