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
pDestroyPiecesAnimation(std::make_shared<Tween>(0,1,3000, Linear::easeIn)),
pFallingPiecesAnimation(std::make_shared<Tween>(0,1,2000, Sine::easeIn))
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

void ColumnsBoardView::RenderTileAt(TileType tileType, int row, int col, Renderer &renderer)
{
    static Position tilePosition;
    
    tilePosition.y = (row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.w;
    tilePosition.x = col * mTileSizePixels.h;
    
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
    
    r.position.y = (row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.w;
    r.position.x = col * mTileSizePixels.h;
    
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

void ColumnsBoardView::RenderPlayerBlock(double dt, Renderer &renderer)
{
    Position pos = Position(pPlayerBlock->position().col * mTileSizePixels.w,
                            (pPlayerBlock->position().row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.h);
    
    Rect playerBlockRect(pos, Size(mTileSizePixels.w, mTileSizePixels.h * pPlayerBlock->size()));
    
    int idxPiecesEnd = pPlayerBlock->size();
    float offset = 0;
    
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
        

        idxPiecesEnd = pPlayerBlock->size()-1;
        pos.y += mTileSizePixels.h - offset;
    }
    
    
    for(int idxPieces = 0; idxPieces < idxPiecesEnd; ++idxPieces)
    {
        const TileType& tileType = pPlayerBlock->operator[](idxPieces);
        auto textureRes = mTile2TextureMapping[tileType];
        
        if (!textureRes.expired())
        {
            auto texture = textureRes.lock()->resource().lock().get();

            renderer.DrawTexture(texture, pos);
        }
        
        pos.y += mTileSizePixels.h;
    }
    
    renderer.SetColor(255,0,255);
    renderer.DrawRectangle(playerBlockRect);
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
    pMovingPiecesAnimation->Start();
}

std::shared_ptr<Tween> ColumnsBoardView::StartDestroyPiecesAnimation(const TilesSet & piecesToDestroyPtr)
{
    // Can't start the same animation twice
    if(pDestroyPiecesAnimation->hasFinished())
    {
        pDestroyPiecesAnimation->Start();
        pPiecesToDestroy = &piecesToDestroyPtr;
    }
    
    return pDestroyPiecesAnimation;
}

std::shared_ptr<Tween> ColumnsBoardView::StartFallingPiecesAnimation(const TilesMovementSet &piecesToMovePtr)
{
    // Can't start the same animation twice
    if(pFallingPiecesAnimation->hasFinished())
    {
        pFallingPiecesAnimation->Start();
        pPiecesToMove = &piecesToMovePtr;
    }
        
    return pFallingPiecesAnimation;
}

