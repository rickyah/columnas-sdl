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
    
    // Receives a PiecesSetStateArgs with the pieces that had moved in the last turn
    // We use that set to search for matches and create the set of pieces that should be destroyed
    void OnSetArgs(std::shared_ptr<IStateArgs> pArgs) override;
    
    void OnEnter() override;
    void OnExit() override;
private:
    
    TilesSet mDestroyedPieces;
    
    void ExtractPositionOfDroppedPieces(TilesSet &destroyedPieces, const TilesMovementSet &piecesMovedDown);
    
    FallingPiecesState(const FallingPiecesState &) = delete;
    FallingPiecesState & operator=(const FallingPiecesState &) = delete;
};

#endif /* FallingPiecesState_hpp */
