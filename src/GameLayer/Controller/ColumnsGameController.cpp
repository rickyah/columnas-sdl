//
//  ColumnsGameController.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#include "ColumnsGameController.hpp"


// Forward declaration
class MovingPiecesState;
class DroppingPiecesState;
class RemovingPiecesState;

void ColumnsGameController::Init()
{
    mColumnsBoard.InitNumEqualPiecesToDestroy(3);
    mColumnsBoard.InitNumRowsGameOver(3);
    mColumnsBoard.InitPlayerBlockInitialPosition(TilePosition(mColumnsBoard.numFirstRowsForGameOver(), mColumnsBoard.columns()/2));

    mColumnsBoardView.InitFirstRowsToSkipWhenRendering(mColumnsBoard.numFirstRowsForGameOver())
        .InitTileSizeInPixels(Size(24,24))
        .InitPieceToTextureMapping({
                {ESpecialBoardPieces::Empty,      mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Tile.png"))},

                {EPieces::Croissant, mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Croissant.png"))},
                {EPieces::Cupcake,   mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Cupcake.png"))},
                {EPieces::Danish,    mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Danish.png"))},
                {EPieces::Donut,     mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Donut.png"))},
                {EPieces::Macaroon,  mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Macaroon.png"))},
                {EPieces::Cookie,    mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/SugarCookie.png"))}
            });
    
    pRandomDistribution = std::make_shared<RandomDistribution>(1,6);
    
    
    // Init FSM
    mFSM.RegisterState(EColumnsGameStatesIds::Moving_Pieces, std::make_shared<MovingPiecesState>(mFSM, *this, mEventQueueRef));
    mFSM.RegisterState(EColumnsGameStatesIds::Removing_Pieces, std::make_shared<RemovingPiecesState>(mFSM, *this));
    mFSM.RegisterState(EColumnsGameStatesIds::Falling_Pieces, std::make_shared<FallingPiecesState>(mFSM, *this));
    
    mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    
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


void ColumnsGameController::PermutePlayerBlockPieces()
{
    mColumnsBoard.MovePlayerBlockPieces();
    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}

void ColumnsGameController::EndGame()
{
    mColumnsBoard.ResetBoardState();
    mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    mFSM.Update(0);
}

bool ColumnsGameController::CanMoveDown() const
{
    return mColumnsBoard.CanMovePlayerBlockDown();
}

bool ColumnsGameController::MoveDown()
{
    return mColumnsBoard.MovePlayerBlockDown();
}

void ColumnsGameController::MoveLeft()
{
    mColumnsBoard.MovePlayerBlockLeft();
}

void ColumnsGameController::MoveRight()
{
    mColumnsBoard.MovePlayerBlockRight();
}

void ColumnsGameController::Update(TimeInfo time)
{
    mFSM.Update(time.dt);

    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}

void ColumnsGameController::Render(TimeInfo time, std::shared_ptr<Renderer> pRenderer)
{
    mColumnsBoardView.Render(time.dt, pRenderer);
    pRenderer->Present();
}

ColumnsGameController::DestroyPiecesInfo ColumnsGameController::StartDestroyingPieces()
{
    auto piecesToDestroy = mColumnsBoard.FindPiecesToDestroy();
    
    
    if(piecesToDestroy.size() >0)
    {
        return std::make_tuple(piecesToDestroy,
                             mColumnsBoardView.StartDestroyPiecesAnimation(piecesToDestroy));
    
    }
    
    return std::make_tuple(piecesToDestroy,
                           nullptr);

}

ColumnsGameController::FallingPiecesInfo ColumnsGameController::StartFallingPieces(TilesSet piecesDestroyed)
{
    auto piecesToMove = mColumnsBoard.FindPiecesToMoveInSubset(piecesDestroyed);
    
    
    if(piecesToMove.size() >0)
    {
        return std::make_tuple(piecesToMove,
                               mColumnsBoardView.StartFallingPiecesAnimation(piecesToMove));
        
    }
    
    return std::make_tuple(piecesToMove,
                           nullptr);
}


void ColumnsGameController::UpdateBoardDestroyPieces(TilesSet piecesToDestroy)
{
    mColumnsBoard.RemovePieces(piecesToDestroy);

    
    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}
    
void ColumnsGameController::UpdateBoardMakePiecesFall(TilesMovementSet piecesToMove)
{
    mColumnsBoard.MovePieces(piecesToMove);
    
    mColumnsBoardView.SetBoardState(mColumnsBoard.boardState());
}
