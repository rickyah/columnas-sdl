//
//  ColumnsGameController.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#include "ColumnsGameController.hpp"


void ColumnsGameController::Init()
{
    mColumnsBoard.InitNumEqualPiecesToDestroy(3);
    mColumnsBoard.InitPlayerBlockInitialPosition(TilePosition(3,3));

    mColumnsBoardView.skipFirstRowsWhenRendering(3);
    
    auto test =   pResourceManager->Get<Texture2dResource>(RESOURCE_ID("Sprites/Tile.png"));

    mColumnsBoardView.SetPieceToTextureMapping(Size(24,24),
        {
            {EPieces::Tile,      pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Tile.png"))},

            {EPieces::Croissant, pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Croissant.png"))},
            {EPieces::Cupcake,   pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Cupcake.png"))},
            {EPieces::Danish,    pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Danish.png"))},
            {EPieces::Donut,     pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Donut.png"))},
            {EPieces::Macaroon,  pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Macaroon.png"))},
            {EPieces::Cookie,    pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/SugarCookie.png"))}
        });
}


void ColumnsGameController::Update(TimeInfo time)
{
    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}

void ColumnsGameController::Render(TimeInfo time, std::shared_ptr<Renderer> pRenderer)
{
    mColumnsBoardView.Render(pRenderer);
    pRenderer->Present();
}