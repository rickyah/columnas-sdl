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
    mColumnsBoard.InitNumRowsGameOver(3);
    
    mColumnsBoard.InitPlayerBlockInitialPosition(TilePosition(mColumnsBoard.numFirstRowsForGameOver(), mColumnsBoard.columns()/2));

    mColumnsBoardView.skipFirstRowsWhenRendering(mColumnsBoard.numFirstRowsForGameOver());
    
    mColumnsBoardView.SetPieceToTextureMapping(Size(24,24),
        {
            {ESpecialBoardPieces::Empty,      pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Tile.png"))},

            {EPieces::Croissant, pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Croissant.png"))},
            {EPieces::Cupcake,   pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Cupcake.png"))},
            {EPieces::Danish,    pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Danish.png"))},
            {EPieces::Donut,     pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Donut.png"))},
            {EPieces::Macaroon,  pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/Macaroon.png"))},
            {EPieces::Cookie,    pResourceManager->Register<Texture2dResource>(RESOURCE_ID("Sprites/SugarCookie.png"))}
        });
    
    pRandomDistribution = std::make_shared<RandomDistribution>(1,6);
    
    
    pEventQueue->AddListener(AppTouch_Event::sEventType, [this](std::shared_ptr<IEventData> event) {
        auto ptr = std::static_pointer_cast<AppTouch_Event>(event);
        
        if (ptr->args().type == AppTouch_Event::ETouchType::Motion)
        {
            SDL_Log("motion (%f,%f)", ptr->args().motion.dx, ptr->args().motion.dy);
        }
        else if (ptr->args().type == AppTouch_Event::ETouchType::Up)
        {
            MovePlayerBlockPieces();
            
            SDL_Log("pos (%d,%d)", ptr->args().pos.x, ptr->args().pos.y);
        }
    });
    
    
    ResetPlayerBlock();
    
    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}


bool ColumnsGameController::ResetPlayerBlock()
{
    return mColumnsBoard.ResetPlayerBlock({
        static_cast<short>(pRandomDistribution->next()),
        static_cast<short>(pRandomDistribution->next()),
        static_cast<short>(pRandomDistribution->next()),
    });
}


void ColumnsGameController::MovePlayerBlockPieces()
{
    mColumnsBoard.MovePlayerBlockPieces();
    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}
void ColumnsGameController::Update(TimeInfo time)
{
    static int32_t passedTime = 0;
    passedTime += time.dt;
    
    bool endGame = false;
    int timePerDropMs = 1000;
    if (passedTime >= timePerDropMs)
    {
        SDL_Log("elapsed: %d, passed %d", time.elapsedMs, passedTime);
        if(!mColumnsBoard.MovePlayerBlockDown())
        {
            if(!ResetPlayerBlock())
            {
                mColumnsBoard.ResetBoardState();
            }
        }
       
        
        // update view
        mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
        passedTime = std::max(0, passedTime -timePerDropMs);
    }
}

void ColumnsGameController::Render(TimeInfo time, std::shared_ptr<Renderer> pRenderer)
{
    mColumnsBoardView.Render(pRenderer);
    pRenderer->Present();
}