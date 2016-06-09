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
#include "DataTypes.hpp"
#include "ResourcesDefinitions.hpp"

struct ViewAnimationState
{
    friend class ColumnsBoardView;
    
    float AnimationTimeMs() { return mTotalAnimationTimeMs;}
    float ElapsedAnimationTimeMS() { return mElapsedAnimationTimeMs;}
    bool IsFinished() { return mElapsedAnimationTimeMs > mTotalAnimationTimeMs; }
    
private:
    float mTotalAnimationTimeMs ;
    float mElapsedAnimationTimeMs = 0;
};

class ColumnsBoardView
{
    typedef std::unordered_map<TileType, std::shared_ptr<Texture2dResource>> TileTypeToTextureMapping;

public:
    explicit ColumnsBoardView();

    void InitPieceToTextureMapping(Size tileSizePixels, TileTypeToTextureMapping mappings);
    void InitDestroyPiecesAnimationTimeMs(double timeMs);
    void InitFallingPiecesAnimationTimeMs(double timeMs);
    
    void SetBoardState( const BoardState & initialState ) { mState = initialState; }
    void skipFirstRowsWhenRendering(uint8_t value) { mSkipRenderingRowsWhenRendering = value; }

    std::shared_ptr<ViewAnimationState> StartDestroyPiecesAnimation(TilesSet piecesToDestroy);
    
    std::shared_ptr<ViewAnimationState> StartFallingPiecesAnimation(TilesMovementSet piecesToMove);
    
    
    void Render(double dt, std::shared_ptr<Renderer> pRenderer);
private:

    bool IsDestroyingPieces() { return !pDestroyPiecesAnimationState->IsFinished(); }
    bool IsMakingPiecesFall() { return !pFallingPiecesAnimationState->IsFinished(); }
    
    void RenderBoard(double dt, std::shared_ptr<Renderer> pRenderer);
    void RenderDestroyAnimation(double dt, std::shared_ptr<Renderer> pRenderer);
    void RenderFallingPiecesAnimation(double dt, std::shared_ptr<Renderer> pRenderer);
    
    std::shared_ptr<ViewAnimationState> pDestroyPiecesAnimationState;
    std::shared_ptr<ViewAnimationState> pFallingPiecesAnimationState;
    
    uint8_t mSkipRenderingRowsWhenRendering;
    TileTypeToTextureMapping mTile2TextureMapping;
    BoardState mState;
    Size mTileSizePixels;
    ColumnsBoardView(const ColumnsBoardView &);
    ColumnsBoardView operator=(const ColumnsBoardView &);
};

#endif /* ColumnsBoardView_hpp */
