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
    
    ColumnsGameController(EventQueue &eventQueue,
                          ResourceManager &resourceMng):
    mColumnsBoard(cDefaultBoardRows, cDefaultBoardColumns, cDefaultEqualPiecesToDestroy),
    mEventQueueRef(eventQueue),
    mResourceManagerRef(resourceMng),
    mColumnsBoardView(&mColumnsBoard, &mPlayerBlock)
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
    enum DestroyPiecesInfoFields{ destroyedPieces, animationState };
    using DestroyPiecesInfo = std::tuple<TilesSet, std::shared_ptr<Tween>>;
    using FallingPiecesInfo = std::tuple<TilesMovementSet, std::shared_ptr<Tween>>;
    
    /* 
     * This checkes the pieces that need to be destroyed and commands the view 
     * to start executing the destroy animation  with these set of pieces
     * The board state is NOT modified by this method
     *
     * @param piecesToDestroy: set of tiles to start searching
     *
     * @returns a tuple<2> containing the set of pieces to destroy and a ViewAnimationState ptr
     * that allows querying for the state of the animation
     */
    DestroyPiecesInfo StartDestroyingPieces(const TilesSet &piecesToDestroy);
    void UpdateBoardDestroyPieces(TilesSet piecesToDestroy);

    FallingPiecesInfo StartFallingPieces(TilesSet piecesDestroyed);
    void ConsolidatePlayerBlock();
    
    void Update(LogicFrameInfo time);
    void Render(RenderFrameInfo time, Renderer &renderer);

    int waitForLongPressMs() const { return 250; }
    float minValueXMotion() const { return 0.02; }
    
private:
    ColumnsGameController(const ColumnsGameController &);
    ColumnsGameController& operator=(const ColumnsGameController &);
    
    ColumnsGameFSM mFSM;
    
    const uint8_t cDefaultEqualPiecesToDestroy = 3;
    const uint8_t cDefaultBoardColumns = 6;
    const uint8_t cDefaultBoardRows = 17;
    
    std::shared_ptr<BitmapFont> pBitmapFont;
    PlayerBlock mPlayerBlock;
    std::vector<TileType> mNextPieces;
    PositionF mPlayerBlockInitialPosition;
    
    ColumnsBoard mColumnsBoard;
    ColumnsBoardView mColumnsBoardView;

    std::shared_ptr<RandomDistribution> pRandomDistribution;
    
    ResourceManager &mResourceManagerRef;
    EventQueue &mEventQueueRef;
};

#endif /* ColumnsGameController_hpp */
