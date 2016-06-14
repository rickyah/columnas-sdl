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
    
    mPlayerBlockInitialPosition = PositionF(mColumnsBoard.numFirstRowsForGameOver(), static_cast<int>(mColumnsBoard.columns()/2));

    // 
    // mColumnsBoard.numEqualPiecesToDestroy(3);
    
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
}


bool ColumnsGameController::ResetPlayerBlock()
{
    mPlayerBlock.SetNewPieces(mNextPieces);
    mPlayerBlock.position(mPlayerBlockInitialPosition);
    
    mNextPieces = {
        static_cast<TileType>(pRandomDistribution->next()),
        static_cast<TileType>(pRandomDistribution->next()),
        static_cast<TileType>(pRandomDistribution->next()),
    };
    return mColumnsBoard.CanMovePlayerBlockTo(TileOffset(), mPlayerBlock);
}


void ColumnsGameController::PermutePlayerBlockPieces()
{
    
    mPlayerBlock.MovePieces();
    mColumnsBoardView.StartAnimatingPlayerBlock();
}

void ColumnsGameController::EndGame()
{
    mColumnsBoard.ResetBoardState();
    mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    mFSM.Update(0);
}

bool ColumnsGameController::CanMoveDown() const
{
    return mColumnsBoard.CanMovePlayerBlockDown(mPlayerBlock);
}

void ColumnsGameController::ConsolidatePlayerBlock()
{
    mColumnsBoard.ConsolidatePlayerBlock(mPlayerBlock);
}

bool ColumnsGameController::MoveDown()
{
    if (mColumnsBoard.CanMovePlayerBlockDown(mPlayerBlock))
    {
        mPlayerBlock.position().row += 0.5;
        return true;
    }
    
    return false;
}

void ColumnsGameController::MoveLeft()
{
    if (mColumnsBoard.CanMovePlayerBlockLeft(mPlayerBlock))
    {
        mPlayerBlock.position().col -= 1;
    }
    
}

void ColumnsGameController::MoveRight()
{
    if (mColumnsBoard.CanMovePlayerBlockRight(mPlayerBlock))
    {
        mPlayerBlock.position().col += 1;
    }
    
}

void ColumnsGameController::Update(LogicFrameInfo time)
{
    mFSM.Update(time.dt);
    mColumnsBoardView.UpdateAnimations(time.dt);
}

void ColumnsGameController::Render(RenderFrameInfo time, std::shared_ptr<Renderer> pRenderer)
{
    mColumnsBoardView.Render(time.interpolation, pRenderer);
    pRenderer->Present();
}

ColumnsGameController::DestroyPiecesInfo ColumnsGameController::StartDestroyingPieces(const TilesSet &piecesToSearch)
{
    if (piecesToSearch.size() > 0)
    {
        return std::make_tuple(mColumnsBoard.FindPiecesToDestroy(piecesToSearch),
                               mColumnsBoardView.StartDestroyPiecesAnimation(piecesToSearch));
    }

    auto piecesToDestroy = mColumnsBoard.FindPiecesToDestroy(mPlayerBlock.occupiedPositions());
    if (piecesToDestroy.size() > 0)
    {
        return std::make_tuple(piecesToDestroy,
                               mColumnsBoardView.StartDestroyPiecesAnimation(piecesToDestroy));
    }
   
    return std::make_tuple(piecesToDestroy, nullptr);
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

}
    
void ColumnsGameController::UpdateBoardMakePiecesFall(TilesMovementSet piecesToMove)
{
    mColumnsBoard.MovePieces(piecesToMove);
}
