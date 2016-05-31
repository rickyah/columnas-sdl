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
#include <map>
#include "Renderer.hpp"
#include "Texture2d.hpp"
#include "ColumnsBoard.hpp"
#include "DataTypes.hpp"

class ColumnsBoardView
{
public:
    explicit ColumnsBoardView(){}

    void SetPieceToTextureMapping(Size tileSizePixels, std::map<TileType, std::shared_ptr<Texture> > mappings);

    void SetBoardState( const BoardState & initialState );

    void Render(std::shared_ptr<Renderer> pRenderer);
private:

    std::map<TileType, std::shared_ptr<Texture>> _tileTypeToTextureMapping;
    BoardState _state;
    Size _tileSizePixels;
    ColumnsBoardView(const ColumnsBoardView &);
    ColumnsBoardView operator=(const ColumnsBoardView &);
};

#endif /* ColumnsBoardView_hpp */
