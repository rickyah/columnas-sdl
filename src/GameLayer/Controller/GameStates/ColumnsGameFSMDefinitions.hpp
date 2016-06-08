//
//  ColumnsGameFSMDefinitions.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#ifndef ColumnsGameFSMDefinitions_hpp
#define ColumnsGameFSMDefinitions_hpp

#include "FSM.hpp"

enum EColumnsGameStatesIds
{
    Moving_Pieces,
    Removing_Pieces,
    Dropping_Pieces
};

typedef FSM<EColumnsGameStatesIds, IState, EnumHasher> ColumnsGameFSM;


// Forward declaration: states need a reference to the controller to perform actions in the game
class ColumnsGameController;

/*
 * Base class for all states in the Columns game
 */
class BaseColumnsGameState : public IState
{
protected:
    BaseColumnsGameState(const ColumnsGameFSM &fsm, ColumnsGameController &controller):
        mFSM(fsm),
        mControllerRef(controller)
    {}
    
    const ColumnsGameFSM &mFSM;
    ColumnsGameController &mControllerRef;
    
private:
    BaseColumnsGameState(const BaseColumnsGameState &);
    BaseColumnsGameState & operator=(const BaseColumnsGameState &);
};



#endif /* ColumnsGameFSMDefinitions_hpp */
