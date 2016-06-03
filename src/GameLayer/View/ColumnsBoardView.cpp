//
//  ColumnsBoardView.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "ColumnsBoardView.hpp"

void ColumnsBoardView::Render(std::shared_ptr<Renderer> pRenderer)
{
    Position offset(0,0);
    Rect r(offset,  mTileSizePixels);
    r.size.w -= 1;
    r.size.h -= 1;
    
    for(auto & row : mState)
    {
        for(TileType & tileType: row)
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

void ColumnsBoardView::SetPieceToTextureMapping(Size tileSizePixels,
                                                TileTypeToTextureMapping mappings)
{
    mTileSizePixels = tileSizePixels;
    mTile2TextureMapping = mappings;
    for(auto const &kvp : mappings) {
        kvp.second->texture()->drawSize(tileSizePixels);
    }
}
