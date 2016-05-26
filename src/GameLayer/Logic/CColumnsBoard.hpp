//
//  CColumnsBoard.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef CColumnsBoard_hpp
#define CColumnsBoard_hpp

#include "CBoard.hpp"

enum EBoardPieces {
    Empty = 0,
    Type1 = 1,
    Type2 = 2
};


class CColumnsBoard : public CBoard
{
public:
    explicit CColumnsBoard(BoardState state):CBoard(state){}
};

#endif /* CColumnsBoard_hpp */
