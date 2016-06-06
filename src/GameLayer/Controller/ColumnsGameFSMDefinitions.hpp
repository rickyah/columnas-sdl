//
//  ColumnsGameFSMDefinitions.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#ifndef ColumnsGameFSMDefinitions_hpp
#define ColumnsGameFSMDefinitions_hpp

enum EColumnsGameStatesIds
{
    Moving_Pieces,
    Removing_Pieces,
    Dropping_Pieces
};


typedef FSM<EColumnsGameStatesIds, IState, EnumHasher> ColumnsGameFSM;

#endif /* ColumnsGameFSMDefinitions_hpp */
