//
//  ColumnsBoardView.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "ColumnsBoardView.hpp"

ColumnsBoardView::ColumnsBoardView()
:pDestroyPiecesAnimationState(std::make_shared<ViewAnimationState>()),
pFallingPiecesAnimationState(std::make_shared<ViewAnimationState>())
{}

void ColumnsBoardView::InitDestroyPiecesAnimationTimeMs(double timeMs)
{
    pDestroyPiecesAnimationState->mTotalAnimationTimeMs = std::max(0.0, timeMs);
}

void ColumnsBoardView::InitFallingPiecesAnimationTimeMs(double timeMs)
{
    pFallingPiecesAnimationState->mTotalAnimationTimeMs = std::max(0.0, timeMs);
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
    }
}
void ColumnsBoardView::RenderBoard(double dt, std::shared_ptr<Renderer> pRenderer)
{
    Position offset(0,0);
    Rect r(offset,  mTileSizePixels);
    r.size.w -= 1;
    r.size.h -= 1;
    
    
    for(int rowIdx = mSkipRenderingRowsWhenRendering ; rowIdx < mState.size(); ++rowIdx)
    {
        
        for(TileType & tileType: mState[rowIdx])
        {

            pRenderer->SetColor(255,255,255);
            r.position = offset;
            pRenderer->FillRectangle(r);

            {
                auto texture = mTile2TextureMapping.find(tileType);

                if (texture != mTile2TextureMapping.end())
                {
                    pRenderer->DrawTextureAt(texture->second->texture(), offset);
                }
            }

            offset.x += mTileSizePixels.w;
        }
        offset.x = 0;
        offset.y += mTileSizePixels.h;
    }
}

void ColumnsBoardView::RenderDestroyAnimation(double dt, std::shared_ptr<Renderer> pRenderer)
{
    pDestroyPiecesAnimationState->mElapsedAnimationTimeMs += dt;
}

void ColumnsBoardView::RenderFallingPiecesAnimation(double dt, std::shared_ptr<Renderer> pRenderer)
{
    pFallingPiecesAnimationState->mElapsedAnimationTimeMs += dt;
}

void ColumnsBoardView::InitPieceToTextureMapping(Size tileSizePixels,
                                                TileTypeToTextureMapping mappings)
{
    mTileSizePixels = tileSizePixels;
    mTile2TextureMapping = mappings;
    for(auto const &kvp : mappings) {
        kvp.second->texture()->drawSize(tileSizePixels);
    }
}


std::shared_ptr<ViewAnimationState> ColumnsBoardView::StartDestroyPiecesAnimation(TilesSet piecesToDestroy)
{
    // Can't start the same animation twice
    if(pDestroyPiecesAnimationState->IsFinished())
    {
        pDestroyPiecesAnimationState->mElapsedAnimationTimeMs = 0;
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

