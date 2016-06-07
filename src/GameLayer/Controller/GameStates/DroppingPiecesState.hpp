//
//  ColumnsGameStates.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#ifndef ColumnsGameStates_hpp
#define ColumnsGameStates_hpp

#include "ColumnsGameFSMDefinitions.hpp"
#include "ColumnsGameController.hpp"

class DroppingPiecesState : public BaseColumnsGameState
{
public:
    DroppingPiecesState(const ColumnsGameFSM &fsm,
                        ColumnsGameController &controller)
    :BaseColumnsGameState(fsm, controller)
    {}
    
    DroppingPiecesState(const DroppingPiecesState &) = delete;
    DroppingPiecesState & operator=(const DroppingPiecesState &) = delete;
};

#endif /* DroppingPiecesState_hpp */
