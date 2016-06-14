//
//  FallingPiecesState.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 6/6/16.
//
//

#ifndef FallingPiecesState_hpp
#define FallingPiecesState_hpp

#include "ColumnsGameFSMDefinitions.hpp"
#include "ColumnsGameController.hpp"
#include "GameStateArgs.hpp"

class FallingPiecesState : public BaseColumnsGameState
{
public:
    FallingPiecesState(const ColumnsGameFSM &fsm,
                        ColumnsGameController &controller)
    :BaseColumnsGameState(fsm, controller)
    {}
    
    void OnSetArgs(std::shared_ptr<IStateArgs> pArgs) override;
    
    void OnEnter() override;
    void OnUpdate(double dt) override;
    void OnExit() override;
private:
    TilesSet mDestroyedPieces;
    TilesMovementSet mPiecesToMove;
    std::shared_ptr<Tween> pAnimationState;
    FallingPiecesState(const FallingPiecesState &);
    FallingPiecesState & operator=(const FallingPiecesState &);
    void ExtractPositionOfDroppedPieces(TilesSet &destroyedPieces, const TilesMovementSet &piecesMovedDown);
};

#endif /* FallingPiecesState_hpp */
