//
//  ColumnsBoardView.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef ColumnsBoardView_hpp
#define ColumnsBoardView_hpp

#include <memory>
#include <unordered_map>
#include <algorithm>

#include "Renderer.hpp"
#include "Texture2d.hpp"
#include "ColumnsBoard.hpp"
#include "PlayerBlock.hpp"
#include "DataTypes.hpp"
#include "ResourcesDefinitions.hpp"
#include "Tween.hpp"


/*
 * Structure that allows to check the state of a view animation.
 * This is intended to be used as a ptr, so the View can actually change 
 * the inner values, and the elements that hold a copy of the pointer to
 * query its state
 */
//struct ViewAnimationState
//{
//    friend class ColumnsBoardView;
//    
//    float AnimationTimeMs() { return mTotalAnimationTimeMs;}
//    float ElapsedAnimationTimeMS() { return mElapsedAnimationTimeMs;}
//    bool IsFinished() { return mElapsedAnimationTimeMs > mTotalAnimationTimeMs; }
//    
//private:
//    float mTotalAnimationTimeMs ;
//    float mElapsedAnimationTimeMs = 0;
//};

class ColumnsBoardView
{
    typedef std::unordered_map<TileType, std::weak_ptr<Texture2dResource>> TileTypeToTextureMapping;

public:
    explicit ColumnsBoardView(const ColumnsBoard * const pColumnsBoardPtr,
                              const PlayerBlock * const playerBlockPtr);

    ColumnsBoardView& InitPieceToTextureMapping(TileTypeToTextureMapping mappings);
    ColumnsBoardView& InitTileSizeInPixels(Size tileSizePixels);
    ColumnsBoardView& InitDestroyPiecesAnimationTimeMs(double timeMs);
    ColumnsBoardView& InitFallingPiecesAnimationTimeMs(double timeMs);
    ColumnsBoardView& InitFirstRowsToSkipWhenRendering(TileCoordinate value);
    
    void StartAnimatingPlayerBlock();
    std::shared_ptr<Tween> StartDestroyPiecesAnimation(const TilesSet &piecesToDestroyPtr);
    std::shared_ptr<Tween> StartFallingPiecesAnimation(const TilesMovementSet &piecesToMovePtr);
    
    void UpdateAnimations(double dt);
    void Render(double framePercent, Renderer &pRenderer);
    
private:

    bool isMovingPieces() { return pMovingPiecesAnimation->isRunning(); }
    bool isDestroyingPieces() { return pDestroyPiecesAnimation->isRunning(); }
    bool isMakingPiecesFall() { return pFallingPiecesAnimation->isRunning(); }
    
    void RenderBoard(double dt, Renderer &renderer);
    void RenderPlayerBlock(double dt, Renderer &renderer);
    void RenderDestroyAnimation(double dt, Renderer &renderer);
    void RenderFallingPiecesAnimation(double dt, Renderer &renderer);
    
    std::shared_ptr<Tween> pMovingPiecesAnimation;
    std::shared_ptr<Tween> pDestroyPiecesAnimation;
    std::shared_ptr<Tween> pFallingPiecesAnimation;
    
    uint8_t mSkipRenderingRowsWhenRendering;
    TileTypeToTextureMapping mTile2TextureMapping;
    
    // Pointers to the model for rendering only (const data);
    const ColumnsBoard * const pColumnsBoard;
    const PlayerBlock * const pPlayerBlock;
    const TilesSet *pPiecesToDestroy;
    const TilesMovementSet *pPiecesToMove;
    
    Size mTileSizePixels;
    
    void RenderTileAt(TileType tileType, int row, int col, Renderer &renderer);
    void RenderEmptyTileAt(int row, int col, Renderer &renderer);
    
    ColumnsBoardView(const ColumnsBoardView &);
    ColumnsBoardView operator=(const ColumnsBoardView &);
};

#endif /* ColumnsBoardView_hpp */
