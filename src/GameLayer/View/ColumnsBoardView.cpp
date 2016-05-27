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
    Rect r(offset,  _tileSizePixels);
    r.size.w -= 1;
    r.size.h -= 1;
    for(auto & row : _state)
    {
        for(TileType & tileType: row)
        {

            pRenderer->SetColor(255,255,255);
            r.position = offset;
            pRenderer->FillRectangle(r);

            {
                auto texture = _tileTypeToTextureMapping.find(tileType);

                if (texture != _tileTypeToTextureMapping.end())
                {
                    pRenderer->DrawTextureAt(texture->second, offset);
                }
            }

            offset.x += _tileSizePixels.w;
        }
        offset.x = 0;
        offset.y += _tileSizePixels.h;
    }
}

void ColumnsBoardView::SetPieceToTextureMapping(Size tileSizePixels,
                                                 std::map<TileType, std::shared_ptr<Texture> > mappings)
{
    _tileSizePixels = tileSizePixels;
    _tileTypeToTextureMapping = mappings;
    for(auto const &kvp : mappings) {
        kvp.second->drawSize(tileSizePixels);
    }
}

void ColumnsBoardView::SetBoardState( const BoardState & initialState)
{
    _state = initialState;
}