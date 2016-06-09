//
//  ColumnsGameController.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#ifndef ColumnsGameController_hpp
#define ColumnsGameController_hpp

#include <memory>
#include <tuple>

#include "ResourceManager.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

#include "RandomDistribution.hpp"

#include "ColumnsBoard.hpp"
#include "ColumnsBoardView.hpp"

#include "ColumnsGameFSMDefinitions.hpp"
#include "MovingPiecesState.hpp"
#include "RemovingPiecesState.hpp"
#include "DroppingPiecesState.hpp"

class ColumnsGameController
{
    enum EPieces
    {
        Croissant   = 1,
        Cupcake     = 2,
        Danish      = 3,
        Donut       = 4,
        Macaroon    = 5,
        Cookie      = 6
    };
    
   
public:
    
    // Use an unscoped enum to std::get fields from the DestroyPiecesInfo tuple instead of using numeric
    // indexers (from Effective Modern C++, Item 10)
    enum DestroyPiecesInfoFields{ destroyedPieces, animationState };
    using DestroyPiecesInfo = std::tuple<TilesSet, std::shared_ptr<ViewAnimationState>>;
    using FallingPiecesInfo = std::tuple<TilesMovementSet, std::shared_ptr<ViewAnimationState>>;
    ColumnsGameController(EventQueue &eventQueue,
                          ResourceManager &resourceMng):
    mColumnsBoard(cDefaultBoardRows, cDefaultBoardColumns),
    mEventQueueRef(eventQueue),
    mResourceManagerRef(resourceMng)
    {}
    
    // TODO: use with a configuration when we read configuration from files
    void Init();
    
    bool CanMoveDown() const;
    
    void EndGame();
    
    bool MoveDown();
    void MoveLeft();
    void MoveRight();
    bool ResetPlayerBlock();
    void PermutePlayerBlockPieces();
    
    void UpdateBoardDestroyPieces(TilesSet piecesToDestroy);
    void UpdateBoardMakePiecesFall(TilesMovementSet piecesToMove);
    DestroyPiecesInfo StartDestroyingPieces();
    FallingPiecesInfo StartFallingPieces(TilesSet piecesDestroyed);
    
    void Update(TimeInfo time);
    void Render(TimeInfo time, std::shared_ptr<Renderer> pRenderer);

    int waitForLongPressMs() const { return 250; }
    float minValueXMotion() const { return 0.02; }
    
private:
    ColumnsGameController(const ColumnsGameController &);
    ColumnsGameController& operator=(const ColumnsGameController &);
    
    ColumnsGameFSM mFSM;
    
    // Creates a new player block with a random set of pieces and positions it at
    // the initial position in the board

//    TilesSet mPiecesToDestroy;
    const uint8_t cDefaultBoardColumns = 6;
    const uint8_t cDefaultBoardRows = 17;
    
    ColumnsBoard mColumnsBoard;
    ColumnsBoardView mColumnsBoardView;

    std::shared_ptr<RandomDistribution> pRandomDistribution;
    
    ResourceManager &mResourceManagerRef;
    EventQueue &mEventQueueRef;
};

#endif /* ColumnsGameController_hpp */
