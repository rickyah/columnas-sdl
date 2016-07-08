	//
//  ColumnsBoardView.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "ColumnsBoardView.hpp"

ColumnsBoardView::ColumnsBoardView(const ColumnsBoard * const pColumnsBoardPtr,
                                   const PlayerBlock * const playerBlockPtr)
:pColumnsBoard(pColumnsBoardPtr),
pPlayerBlock(playerBlockPtr),
pMovingPiecesAnimation(std::make_shared<Tween>(0,1,100, Sine::easeIn)),
pDestroyPiecesAnimation(std::make_shared<Tween>(0,1,1000, Linear::easeIn)),
pFallingPiecesAnimation(std::make_shared<Tween>(0,1,1000, Sine::easeIn))
{}

ColumnsBoardView& ColumnsBoardView::InitPieceToTextureMapping(TileTypeToTextureMapping mappings)
{
    mTile2TextureMapping = mappings;

    for(auto const &kvp : mTile2TextureMapping) {
        kvp.second.lock()->resource().lock()->drawSize(mTileSizePixels);
    }
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitTileSizeInPixels(Size tileSizePixels)
{
    mTileSizePixels = tileSizePixels;
    
    for(auto const &kvp : mTile2TextureMapping) {
        kvp.second.lock()->resource().lock()->drawSize(mTileSizePixels);
    }
    
    pMovingPiecesAnimation->initialValue(0).endValue(mTileSizePixels.h).durationMs(pMovingPiecesAnimation->durationMs()).Start();
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitFallingPiecesAnimationTimeMs(double timeMs)
{
    pFallingPiecesAnimation->Start();
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitDestroyPiecesAnimationTimeMs(double timeMs)
{
    pFallingPiecesAnimation->Start();
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitFirstRowsToSkipWhenRendering(TileCoordinate value)
{
    mSkipRenderingRowsWhenRendering = value;
    
    return *this;
}
void ColumnsBoardView::UpdateAnimations(double dt)
{
    pMovingPiecesAnimation->Update(dt);
    pDestroyPiecesAnimation->Update(dt);
    pFallingPiecesAnimation->Update(dt);
}
                        
void ColumnsBoardView::Render(double framePercent, Renderer &rendererRef)
{
    if (isDestroyingPieces())
    {
        RenderDestroyAnimation(framePercent, rendererRef);
    }
    else if (isMakingPiecesFall())
    {
        RenderFallingPiecesAnimation(framePercent, rendererRef);
    }
    else
    {
        RenderBoard(framePercent, rendererRef);
        RenderPlayerBlock(framePercent, rendererRef);
    }
}

Position ColumnsBoardView::GetPositionForCoordinates(int row, int col)
{
    return {
        mBoardRenderOffset.x + col * mTileSizePixels.w,
        mBoardRenderOffset.y + (row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.h
    };
}

void ColumnsBoardView::RenderTileAt(TileType tileType, int row, int col, Renderer &renderer)
{
    Position tilePosition = GetPositionForCoordinates(row, col);
    
    auto textureRes = mTile2TextureMapping[tileType];
    
    if (!textureRes.expired())
    {
        auto texture = textureRes.lock()->resource().lock().get();
        renderer.DrawTexture(texture, tilePosition);
    }
}

void ColumnsBoardView::RenderEmptyTileAt(int row, int col, Renderer &renderer)
{
    static Rect r;
    
    r.position = GetPositionForCoordinates(row, col);
    
    r.size.w = mTileSizePixels.w - 1;
    r.size.h = mTileSizePixels.h - 1;
    
    renderer.SetColor(255,255,255);
    renderer.DrawFillRectangle(r);
    auto textureRes = mTile2TextureMapping[ESpecialBoardPieces::Empty];
    
    if (!textureRes.expired())
    {
        auto texture = textureRes.lock()->resource().lock().get();
        renderer.DrawTexture(texture, r.position);
    }

}

void ColumnsBoardView::RenderBoard(double dt, Renderer &renderer)
{
    Rect r(Position(),  mTileSizePixels);
    r.size.w -= 1;
    r.size.h -= 1;
    
    const ColumnsBoard & board = (*pColumnsBoard);

    for(std::size_t rowIdx = mSkipRenderingRowsWhenRendering ; rowIdx < board.rows(); ++rowIdx)
    {
        for(std::size_t colIdx = 0; colIdx < board.columns(); ++colIdx)
        {
            // We use an empty tile as bg
            RenderEmptyTileAt(rowIdx, colIdx, renderer);
            
            // Draw piece only if the tile at this position is not empty
            const TileType& tileType = board[rowIdx][colIdx];
            if (tileType == ESpecialBoardPieces::Empty) continue;
            
            RenderTileAt(tileType, rowIdx, colIdx, renderer);
        }
        
    }
}

void ColumnsBoardView::RenderNextPieces(const std::vector<TileType> & tiles, Renderer& renderer)
{
    int boardRightEdge = pColumnsBoard->columns() * mTileSizePixels.w + mBoardRenderOffset.x;

    Rect nextPiecesRect = Rect(Position(boardRightEdge, mBoardRenderOffset.y),
                               Size(mTileSizePixels.w, mTileSizePixels.h * tiles.size()));

    renderer.SetColor(0,0,0);
    renderer.DrawFillRectangle(nextPiecesRect);
    RenderPlayerTilesAtPos(tiles, nextPiecesRect.position, renderer, 0, tiles.size());
}


void ColumnsBoardView::RenderPlayerBlock(double dt, Renderer &renderer)
{
    if (pPlayerBlock->size() <= 0)
    {
        Logger::log.Warn("Trying to render an empty player block");
        return;
    }

    // Don't use GetPositionForCoordinates because the block drops can be positioned at the half part of a column
    Position pos = Position(
        mBoardRenderOffset.x + pPlayerBlock->position().col * mTileSizePixels.w,
        mBoardRenderOffset.y + (pPlayerBlock->position().row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.h);
    
    Rect playerBlockRect(pos, Size(mTileSizePixels.w, mTileSizePixels.h * pPlayerBlock->size()));
    
    float offset = 0;
    
    // If we are animating the movement of the pieces inside the player block
    // then we must draw the last piece (the one at the bottom) partially in the
    // bottom of the block and in the top of it
    if(pMovingPiecesAnimation->isRunning())
    {
        offset = pMovingPiecesAnimation->currentValue();
        
        const TileType& tileType = pPlayerBlock->operator[](pPlayerBlock->size()-1);
        
        
        auto textureRes = mTile2TextureMapping[tileType];
        
        if (!textureRes.expired())
        {
            auto texture = textureRes.lock()->resource().lock().get();
            
            renderer.DrawTexture(texture, pos, Rect(0, offset, mTileSizePixels.w, mTileSizePixels.h - offset));
        
            pos.y += (pPlayerBlock->size()) * mTileSizePixels.h - offset;
            
            renderer.DrawTexture(texture, pos, Rect(0, 0, mTileSizePixels.w, offset));
            
            pos.y -= (pPlayerBlock->size()) * mTileSizePixels.h - offset;
        }
        
        pos.y += mTileSizePixels.h - offset;

        // Don't draw the last piece
        RenderPlayerTilesAtPos(pPlayerBlock->pieces(), pos, renderer, 0, pPlayerBlock->size()-1);
    }
    else
    {
        RenderPlayerTilesAtPos(pPlayerBlock->pieces(), pos, renderer, 0, pPlayerBlock->size());
    }
    
    renderer.SetColor(255,0,255);
    renderer.DrawRectangle(playerBlockRect);
}

void ColumnsBoardView::RenderPlayerTilesAtPos(const std::vector<TileType> &pieces, Position pos, Renderer &renderer, size_t from, size_t to)
{
    for(size_t idxPieces = from; idxPieces < to; ++idxPieces)
    {
        const TileType& tileType = pieces[idxPieces];
        auto textureRes = mTile2TextureMapping[tileType];
        
        if (!textureRes.expired())
        {
            auto texture = textureRes.lock()->resource().lock().get();

            renderer.DrawTexture(texture, pos);
        }
        
        pos.y += mTileSizePixels.h;
    }
}


void ColumnsBoardView::RenderDestroyAnimation(double framePercent, Renderer &rendererRef)
{
//    pDestroyPiecesAnimation->currentValue()
}

void ColumnsBoardView::RenderFallingPiecesAnimation(double framePercent, Renderer &rendererRef)
{
    
}


void ColumnsBoardView::StartAnimatingPlayerBlock()
{
    pMovingPiecesAnimation->Restart();
}

void ColumnsBoardView::StartDestroyPiecesAnimation(const TilesSet &piecesToDestroy, std::function<void()> endCallback)
{
    pDestroyPiecesAnimation->Stop();
    
    pDestroyPiecesAnimation->endCallback([endCallback](const Tween&) {
        if(endCallback) endCallback();
    });
    pDestroyPiecesAnimation->Start();
    
    pPiecesToDestroy = &piecesToDestroy;

}

void ColumnsBoardView::StartFallingPiecesAnimation(const TilesMovementSet &piecesToMovePtr, std::function<void()> endCallback)
{
    // Can't start the same animation twice
    pFallingPiecesAnimation->Stop();
    pFallingPiecesAnimation->endCallback([endCallback](const Tween&) {
        if(endCallback) endCallback();
    });
    pFallingPiecesAnimation->Start();

    pPiecesToMove = &piecesToMovePtr;
}

