//
//  CColumnsBoardView.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef CColumnsBoardView_hpp
#define CColumnsBoardView_hpp

#include <memory>
#include <map>
#include "Renderer.hpp"
#include "Texture.hpp"
#include "ColumnsBoard.hpp"
#include "DataTypes.hpp"

class CColumnsBoardView
{
public:
    explicit CColumnsBoardView(){}

    void SetPieceToTextureMapping(Size tileSizePixels, std::map<TileType, std::shared_ptr<Texture> > mappings);

    void SetBoardState( const BoardState & initialState );

    void Render(std::shared_ptr<Renderer> pRenderer);
private:

    std::map<TileType, std::shared_ptr<Texture>> _tileTypeToTextureMapping;
    BoardState _state;
    Size _tileSizePixels;
    CColumnsBoardView(const CColumnsBoardView &);
    CColumnsBoardView operator=(const CColumnsBoardView &);
};

#endif /* CColumnsBoardView_hpp */
