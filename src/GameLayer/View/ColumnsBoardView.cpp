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
pMovingPiecesAnimation(std::make_shared<Tween>(0,1,100, Linear::easeIn)),
pDestroyPiecesAnimation(std::make_shared<Tween>(0,1,3000, Linear::easeIn)),
pFallingPiecesAnimation(std::make_shared<Tween>(0,1,2000, Sine::easeIn))
{}

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
    
    pMovingPiecesAnimation->Reset(0, mTileSizePixels.h, pMovingPiecesAnimation->durationMs());
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitFallingPiecesAnimationTimeMs(double timeMs)
{
    pFallingPiecesAnimation->Reset(0,1, timeMs);
    
    return *this;
}

ColumnsBoardView& ColumnsBoardView::InitDestroyPiecesAnimationTimeMs(double timeMs)
{
    pDestroyPiecesAnimation->Reset(0,1,timeMs);
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
                        
void ColumnsBoardView::Render(double framePercent, std::shared_ptr<Renderer> pRenderer)
{
    if (isDestroyingPieces())
    {
        RenderDestroyAnimation(framePercent, pRenderer);
    }
    else if (isMakingPiecesFall())
    {
        RenderFallingPiecesAnimation(framePercent, pRenderer);
    }
    else
    {
        RenderBoard(framePercent, pRenderer);
        RenderPlayerBlock(framePercent, pRenderer);
    }
}


void ColumnsBoardView::RenderBoard(double dt, std::shared_ptr<Renderer> pRenderer)
{
    Rect r(Position(),  mTileSizePixels);
    r.size.w -= 1;
    r.size.h -= 1;
    
    const ColumnsBoard & board = (*pColumnsBoard);

    for(std::size_t rowIdx = mSkipRenderingRowsWhenRendering ; rowIdx < board.rows(); ++rowIdx)
    {
        r.position.y = (rowIdx - mSkipRenderingRowsWhenRendering) * mTileSizePixels.w;
        
        for(std::size_t colIdx = 0; colIdx < board.columns(); ++ colIdx)
        {
            r.position.x = colIdx * mTileSizePixels.h;
            
            // Draw bg
            pRenderer->SetColor(255,255,255);
            pRenderer->FillRectangle(r);
            auto textureRes = mTile2TextureMapping[ESpecialBoardPieces::Empty];
            if (!textureRes.expired())
            {
                auto texture = textureRes.lock()->texture();
                pRenderer->DrawTexture(texture, r.position);
            }
            
            // Draw piece only if the boards at this position is not empty
            const TileType& tileType = board[rowIdx][colIdx];
            if (tileType == ESpecialBoardPieces::Empty) continue;
            
            textureRes = mTile2TextureMapping[tileType];

            if (!textureRes.expired())
            {
                auto texture = textureRes.lock()->texture();
                pRenderer->DrawTexture(texture, r.position);
            }

        }
        
    }
}

void ColumnsBoardView::RenderPlayerBlock(double dt, std::shared_ptr<Renderer> pRenderer)
{
    Position pos = Position(pPlayerBlock->position().col * mTileSizePixels.w,
                            (pPlayerBlock->position().row - mSkipRenderingRowsWhenRendering) * mTileSizePixels.h);
    

    int idxPiecesEnd = pPlayerBlock->size();
    float offset = 0;
    
    if(pMovingPiecesAnimation->isRunning())
    {
        offset = pMovingPiecesAnimation->currentValue();
        
        const TileType& tileType = pPlayerBlock->operator[](pPlayerBlock->size()-1);
        
        
        auto textureRes = mTile2TextureMapping[tileType];
        
        if (!textureRes.expired())
        {
            auto texture = textureRes.lock()->texture();
            
            pRenderer->DrawTexture(texture, pos, Rect(0, offset, mTileSizePixels.w, mTileSizePixels.h - offset));
        
            pos.y += (pPlayerBlock->size()) * mTileSizePixels.h - offset;
            
            pRenderer->DrawTexture(texture, pos, Rect(0, 0, mTileSizePixels.w, offset));
            
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
            auto texture = textureRes.lock()->texture();

            pRenderer->DrawTexture(texture, pos);
        }
        
        pos.y += mTileSizePixels.h;
    }
    
}


void ColumnsBoardView::RenderDestroyAnimation(double dt, std::shared_ptr<Renderer> pRenderer)
{
    
}

void ColumnsBoardView::RenderFallingPiecesAnimation(double dt, std::shared_ptr<Renderer> pRenderer)
{
    
}


void ColumnsBoardView::StartAnimatingPlayerBlock()
{
    pMovingPiecesAnimation->Restart();
}

std::shared_ptr<Tween> ColumnsBoardView::StartDestroyPiecesAnimation(TilesSet piecesToDestroy)
{
    // Can't start the same animation twice
    if(pDestroyPiecesAnimation->hasFinished())
    {
        pDestroyPiecesAnimation->Restart();
    }
    
    return pDestroyPiecesAnimation;
}

std::shared_ptr<Tween> ColumnsBoardView::StartFallingPiecesAnimation(TilesMovementSet piecesToMove)
{
    // Can't start the same animation twice
    if(pFallingPiecesAnimation->hasFinished())
    {
        pFallingPiecesAnimation->Restart();
    }
        
    return pFallingPiecesAnimation;
}

