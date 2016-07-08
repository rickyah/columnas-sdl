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
#include <functional>

#include "ResourceManager.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

#include "RandomDistribution.hpp"

#include "ColumnsBoard.hpp"
#include "ColumnsBoardView.hpp"

#include "ColumnsGameFSMDefinitions.hpp"
#include "MovingPiecesState.hpp"
#include "RemovingPiecesState.hpp"
#include "FallingPiecesState.hpp"

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
    
    ColumnsGameController(EventQueue &eventQueue,
                          ResourceManager &resourceMng):
    mColumnsBoardModel(cDefaultBoardRows, cDefaultBoardColumns, cDefaultEqualPiecesToDestroy),
    mEventQueueRef(eventQueue),
    mResourceManagerRef(resourceMng),
    mColumnsBoardView(&mColumnsBoardModel, &mPlayerBlock)
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
    
    
    void UpdateBoardMakePiecesFall(TilesMovementSet piecesToMove);
    
    
    // Use an unscoped enum to std::get fields from the DestroyPiecesInfo tuple instead of using numeric
    // indexers (from Effective Modern C++, Item 10)
//    enum DestroyPiecesInfoFields{ pieces, animationState };
//    using DestroyPiecesInfo = std::tuple<TilesSet&, std::weak_ptr<Tween>>;
//    using FallingPiecesInfo = std::tuple<TilesMovementSet&, std::weak_ptr<Tween>>;
    
    /* 
     * This checkes the pieces that need to be destroyed and commands the view 
     * to start executing the destroy animation  with these set of pieces
     * The board state is NOT modified by this method
     *
     * @param piecesToSearch: set of tiles to start searching
     * @param endCallback: callback triggered after the animations has finished playing
     * @returns a tuple<2> containing the set of pieces to destroy and a ViewAnimationState ptr
     * that allows querying for the state of the animation
     */
    void StartDestroyingPieces(const TilesSet &piecesToSearch, std::function<void(TilesSet &)> endCallback = nullptr);
    
    void UpdateBoardDestroyPieces(TilesSet piecesToDestroy);

    void StartFallingPieces(TilesSet piecesDestroyed, std::function<void(TilesMovementSet &)> endCallback = nullptr);
    void ConsolidatePlayerBlock();
    
    void Update(LogicFrameInfo time);
    void Render(RenderFrameInfo time, Renderer &renderer);

    int waitForLongPressMs() const { return 250; }
    float minValueXMotion() const { return 0.02; }
    
private:
    ColumnsGameController(const ColumnsGameController &) = delete;
    ColumnsGameController& operator=(const ColumnsGameController &) = delete;

    // Default config values
    const uint8_t cDefaultEqualPiecesToDestroy = 3;
    const uint8_t cDefaultBoardColumns = 6;
    const uint8_t cDefaultBoardRows = 17;

    // Model
    ColumnsBoard mColumnsBoardModel;

    // View
    ColumnsBoardView mColumnsBoardView;
    
    // Game State machine
    ColumnsGameFSM mFSM;

    
    // Player block data
    PlayerBlock mPlayerBlock;
    std::vector<TileType> mNextPieces;
    PositionF mPlayerBlockInitialPosition;
    
    // Random number generator
    std::shared_ptr<RandomDistribution> pRandomDistribution;
    
    // Dependencies
    ResourceManager &mResourceManagerRef;
    EventQueue &mEventQueueRef;
};

#endif /* ColumnsGameController_hpp */
