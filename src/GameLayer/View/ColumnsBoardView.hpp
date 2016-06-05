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
#include "Renderer.hpp"
#include "Texture2d.hpp"
#include "ColumnsBoard.hpp"
#include "DataTypes.hpp"
#include "ResourcesDefinitions.hpp"


class ColumnsBoardView
{
    typedef std::unordered_map<TileType, std::shared_ptr<Texture2dResource>> TileTypeToTextureMapping;

public:
    explicit ColumnsBoardView(){}

    void SetPieceToTextureMapping(Size tileSizePixels, TileTypeToTextureMapping mappings);

    void SetBoardState( const BoardState & initialState ) { mState = initialState; }

    void skipFirstRowsWhenRendering(uint8_t value) { mSkipRenderingRowsWhenRendering = value; }
    
    void Render(std::shared_ptr<Renderer> pRenderer);
private:

    
    uint8_t mSkipRenderingRowsWhenRendering;
    TileTypeToTextureMapping mTile2TextureMapping;
    BoardState mState;
    Size mTileSizePixels;
    ColumnsBoardView(const ColumnsBoardView &);
    ColumnsBoardView operator=(const ColumnsBoardView &);
};

#endif /* ColumnsBoardView_hpp */
