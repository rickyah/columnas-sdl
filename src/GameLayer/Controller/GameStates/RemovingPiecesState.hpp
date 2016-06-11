//
//  RemovingPiecesState.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 7/6/16.
//
//

#ifndef RemovingPiecesState_hpp
#define RemovingPiecesState_hpp

#include "ColumnsGameFSMDefinitions.hpp"
#include "ColumnsGameController.hpp"

/*
 * Search and destroy the pieces that can be removed after a block has been placed
 */
class RemovingPiecesState : public BaseColumnsGameState
{
public:
    RemovingPiecesState(const ColumnsGameFSM &fsm,
                        ColumnsGameController &controller)
    :BaseColumnsGameState(fsm, controller)
    {}
    
    void OnSetArgs(std::shared_ptr<IStateArgs> pArgs) override;
    virtual void OnEnter() override;
    virtual void OnUpdate(double dt) override;
    virtual void OnExit() override;
private:
    TilesSet mPiecesToDestroy;
    std::shared_ptr<ViewAnimationState> pAnimationState;
};


#endif /* RemovingPiecesState_hpp */
