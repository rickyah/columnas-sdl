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
pDestroyPiecesAnimationState(std::make_shared<ViewAnimationState>()),
pFallingPiecesAnimationState(std::make_shared<ViewAnimationState>())
{}

ColumnsBoardView& ColumnsBoardView::InitDestroyPiecesAnimationTimeMs(double timeMs)
{
    pDestroyPiecesAnimationState->mTotalAnimationTimeMs = std::max(0.0, timeMs);

    return *this;
}


ColumnsBoardView& ColumnsBoardView::InitPieceToTextureMapping(TileTypeToTextureMapping mappings)
{
    mTile2TextureMapping = mappings;

    for(auto const &kvp : mTile2TextureMapping) {
        kvp.second.lock()->texture()->drawSize(mTileSizePixels);
    }
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitTileSizeInPixels(Size tileSizePixels)
{
    mTileSizePixels = tileSizePixels;
    
    for(auto const &kvp : mTile2TextureMapping) {
        kvp.second.lock()->texture()->drawSize(mTileSizePixels);
    }
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitFallingPiecesAnimationTimeMs(double timeMs)
{
    pFallingPiecesAnimationState->mTotalAnimationTimeMs = std::max(0.0, timeMs);
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitFirstRowsToSkipWhenRendering(TileCoordinate value)
{
    mSkipRenderingRowsWhenRendering = value;
    
    return *this;
}

void ColumnsBoardView::Render(double dt, std::shared_ptr<Renderer> pRenderer)
{
    if (IsDestroyingPieces())
    {
        RenderDestroyAnimation(dt, pRenderer);
    }
    else if (IsMakingPiecesFall())
    {
        RenderFallingPiecesAnimation(dt, pRenderer);
    }
    else
    {
        RenderBoard(dt, pRenderer);
        RenderPlayerBlock(dt, pRenderer);
    }
}

void ColumnsBoardView::RenderFallingPiecesAnimation(double dt, std::shared_ptr<Renderer> pRenderer)
{
    pFallingPiecesAnimationState->mElapsedAnimationTimeMs += dt;
}


void ColumnsBoardView::RenderBoard(double dt, std::shared_ptr<Renderer> pRenderer)
{
    Position offset(0,0);
    Rect r(offset,  mTileSizePixels);
    r.size.w -= 1;
    r.size.h -= 1;
    
    const ColumnsBoard & board = (*pColumnsBoard);
    
    for(std::size_t rowIdx = mSkipRenderingRowsWhenRendering ; rowIdx < board.rows(); ++rowIdx)
    {
        for(std::size_t colIdx = 0; colIdx < board.columns(); ++ colIdx)
        {
            const TileType& tileType = board[rowIdx][colIdx];            

            pRenderer->SetColor(255,255,255);
            r.position = offset;
            pRenderer->FillRectangle(r);

            {
                auto texture = mTile2TextureMapping.find(tileType);

                if (texture != mTile2TextureMapping.end())
                {
                    pRenderer->DrawTextureAt(texture->second.lock()->texture(), offset);
                }
            }

            offset.x += mTileSizePixels.w;
        }
        offset.x = 0;
        offset.y += mTileSizePixels.h;
    }
}

void ColumnsBoardView::RenderPlayerBlock(double dt, std::shared_ptr<Renderer> pRenderer)
{
    // x refers to rows, y to columns
    Position pos = Position(pPlayerBlock->position().col * mTileSizePixels.w,
                            (pPlayerBlock->position().row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.h);
    
    for(auto idxPieces = 0; idxPieces < pPlayerBlock->size(); ++idxPieces)
    {
        const TileType& tileType = pPlayerBlock->operator[](idxPieces);
        
        auto texture = mTile2TextureMapping.find(tileType);
        
        if (texture != mTile2TextureMapping.end())
        {
            pRenderer->DrawTextureAt(texture->second.lock()->texture(), pos);
        }
        
        pos.y += mTileSizePixels.h;
    }
}


void ColumnsBoardView::RenderDestroyAnimation(double dt, std::shared_ptr<Renderer> pRenderer)
{
    
}

std::shared_ptr<ViewAnimationState> ColumnsBoardView::StartDestroyPiecesAnimation(TilesSet piecesToDestroy)
{
    // Can't start the same animation twice
    if(pDestroyPiecesAnimationState->IsFinished())
    {
        pDestroyPiecesAnimationState->mElapsedAnimationTimeMs = 0;
        pDestroyPiecesAnimationState->mTotalAnimationTimeMs = 100;
    }
    
    return pDestroyPiecesAnimationState;
}

std::shared_ptr<ViewAnimationState> ColumnsBoardView::StartFallingPiecesAnimation(TilesMovementSet piecesToMove)
{
    // Can't start the same animation twice
    if(pFallingPiecesAnimationState->IsFinished())
    {
        pFallingPiecesAnimationState->mElapsedAnimationTimeMs = 0;
    }
        
    return pFallingPiecesAnimationState;
}

