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
    void StartDestroyPiecesAnimation(const TilesSet &piecesToDestroyPtr, std::function<void()> endCallback = nullptr);
    void StartFallingPiecesAnimation(const TilesMovementSet &piecesToMovePtr, std::function<void()> endCallback = nullptr);
    
    void UpdateAnimations(double dt);
    void RenderNextPieces(const std::vector<TileType> & tiles, Renderer& renderer);
    void Render(double framePercent, Renderer &pRenderer);
    
private:

    Position GetPositionForCoordinates(int row, int col) const;
    Position GetPositionForCoordinates(int row, int col, const Size &drawSize) const;
    bool isMovingPieces() { return pMovingPiecesAnimation->isRunning(); }
    bool isDestroyingPieces() { return pDestroyPiecesAnimation->isRunning(); }
    bool isMakingPiecesFall() { return pFallingPiecesAnimation->isRunning(); }
    
    void RenderBoard(double dt, Renderer &renderer);
    void RenderPlayerTilesAtPos(const std::vector<TileType> &pieces, Position pos, Renderer &renderer, size_t from, size_t to);
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
    TilesSet mPiecesToDestroy;
    TilesMovementSet mPiecesToMove;
    
    Size mTileSizePixels;
    Position mBoardRenderOffset = {20, 60};
    void RenderTileAt(TileType tileType, int row, int col, Renderer &renderer);
    void RenderEmptyTileAt(int row, int col, Renderer &renderer);
    
    ColumnsBoardView(const ColumnsBoardView &);
    ColumnsBoardView operator=(const ColumnsBoardView &);
};

#endif /* ColumnsBoardView_hpp */
