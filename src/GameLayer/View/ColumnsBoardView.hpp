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
    std::shared_ptr<Tween> StartDestroyPiecesAnimation(TilesSet piecesToDestroy);
    std::shared_ptr<Tween> StartFallingPiecesAnimation(TilesMovementSet piecesToMove);
    
    void UpdateAnimations(double dt);
    void Render(double framePercent, std::shared_ptr<Renderer> pRenderer);
    
private:

    bool isMovingPieces() { return pMovingPiecesAnimation->isRunning(); }
    bool isDestroyingPieces() { return pDestroyPiecesAnimation->isRunning(); }
    bool isMakingPiecesFall() { return pFallingPiecesAnimation->isRunning(); }
    
    void RenderBoard(double dt, std::shared_ptr<Renderer> pRenderer);
    void RenderPlayerBlock(double dt, std::shared_ptr<Renderer> pRenderer);
    void RenderDestroyAnimation(double dt, std::shared_ptr<Renderer> pRenderer);
    void RenderFallingPiecesAnimation(double dt, std::shared_ptr<Renderer> pRenderer);
    
    std::shared_ptr<Tween> pMovingPiecesAnimation;
    std::shared_ptr<Tween> pDestroyPiecesAnimation;
    std::shared_ptr<Tween> pFallingPiecesAnimation;
    
    uint8_t mSkipRenderingRowsWhenRendering;
    TileTypeToTextureMapping mTile2TextureMapping;
    
    const ColumnsBoard * const pColumnsBoard;
    const PlayerBlock * const pPlayerBlock;
    
    Size mTileSizePixels;

    void RenderTileAt(TileType tileType, int row, int col, std::shared_ptr<Renderer> pRenderer);
    void RenderEmptyTileAt(int row, int col, std::shared_ptr<Renderer> pRenderer);

    ColumnsBoardView(const ColumnsBoardView &);
    ColumnsBoardView operator=(const ColumnsBoardView &);
};

#endif /* ColumnsBoardView_hpp */
