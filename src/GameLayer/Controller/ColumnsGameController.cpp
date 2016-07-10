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
    
    mPlayerBlockInitialPosition = PositionF(mColumnsBoardModel.numFirstRowsForGameOver(), static_cast<int>(mColumnsBoardModel.columns()/2));

    // 
    // mColumnsBoardModel.numEqualPiecesToDestroy(3);
    
    mColumnsBoardView.InitFirstRowsToSkipWhenRendering(mColumnsBoardModel.numFirstRowsForGameOver())
        .InitTileSizeInPixels(Size(24,24))
        .InitPieceToTextureMapping({
            
                {ESpecialBoardPieces::Empty,      mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Tile.png"))},

                {EPieces::Croissant,    mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Croissant.png"))},
                {EPieces::Cupcake,      mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Cupcake.png"))},
                {EPieces::Danish,       mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Danish.png"))},
                {EPieces::Donut,        mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Donut.png"))},
                {EPieces::Macaroon,     mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/Macaroon.png"))},
                {EPieces::Cookie,       mResourceManagerRef.Register<Texture2dResource>(RESOURCE_ID("Sprites/SugarCookie.png"))}
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
    mPlayerBlock.pieces(mNextPieces);
    mPlayerBlock.position(mPlayerBlockInitialPosition);
    
    mNextPieces = {
        static_cast<TileType>(pRandomDistribution->next()),
        static_cast<TileType>(pRandomDistribution->next()),
        static_cast<TileType>(pRandomDistribution->next()),
    };
    return mColumnsBoardModel.CanMovePlayerBlockTo(TileOffset(), mPlayerBlock);
}


void ColumnsGameController::PermutePlayerBlockPieces()
{
    
    mPlayerBlock.MovePieces();
    mColumnsBoardView.StartAnimatingPlayerBlock();
}

void ColumnsGameController::EndGame()
{
    mColumnsBoardModel.ResetBoardState();
    mFSM.ChangeTo(EColumnsGameStatesIds::Moving_Pieces);
    mFSM.Update(0);
}

bool ColumnsGameController::CanMoveDown() const
{
    return mColumnsBoardModel.CanMovePlayerBlockDown(mPlayerBlock);
}

void ColumnsGameController::ConsolidatePlayerBlock()
{
    mColumnsBoardModel.ConsolidatePlayerBlock(mPlayerBlock);
}

bool ColumnsGameController::MoveDown()
{
    if (mColumnsBoardModel.CanMovePlayerBlockDown(mPlayerBlock))
    {
        mPlayerBlock.position().row += 0.5;
        return true;
    }
    
    return false;
}

void ColumnsGameController::MoveLeft()
{
    if (mColumnsBoardModel.CanMovePlayerBlockLeft(mPlayerBlock))
    {
        mPlayerBlock.position().col -= 1;
    }
    
}

void ColumnsGameController::MoveRight()
{
    if (mColumnsBoardModel.CanMovePlayerBlockRight(mPlayerBlock))
    {
        mPlayerBlock.position().col += 1;
    }
    
}

void ColumnsGameController::Update(LogicFrameInfo time)
{
    mFSM.Update(time.dt);
    mColumnsBoardView.UpdateAnimations(time.dt);
}

void ColumnsGameController::Render(RenderFrameInfo time, Renderer &renderer)    
{
    mColumnsBoardView.Render(time.interpolation, renderer);
    mColumnsBoardView.RenderNextPieces(this->mNextPieces, renderer);
}

void ColumnsGameController::StartDestroyingPieces(const TilesSet &piecesToSearch,
                                                  std::function<void(TilesSet &)> endCallback)
{
    TilesSet piecesToDestroy = mColumnsBoardModel.FindPiecesToDestroy(piecesToSearch.size() > 0 ? piecesToSearch
                                                                                            : mPlayerBlock.occupiedPositions());
    if (piecesToDestroy.size() > 0)
    {
        mColumnsBoardView.StartDestroyPiecesAnimation(piecesToDestroy, [this, piecesToDestroy, endCallback]() mutable {
            UpdateBoardDestroyPieces(piecesToDestroy);
            if(endCallback) endCallback(piecesToDestroy);
        });
    }
    else
    {
        if(endCallback) endCallback(piecesToDestroy);
    }

    mPlayerBlock.ClearPieces();
}

void ColumnsGameController::StartFallingPieces(TilesSet piecesDestroyed, std::function<void(TilesMovementSet &)> endCallback)
{
    TilesMovementSet piecesToMove = mColumnsBoardModel.FindPiecesToMoveInSubset(piecesDestroyed);
    
    if(piecesToMove.size() > 0)
    {
        mColumnsBoardView.StartFallingPiecesAnimation(piecesToMove, [this, piecesToMove, endCallback]() mutable{
            UpdateBoardMakePiecesFall(piecesToMove);
            if(endCallback) endCallback(piecesToMove);
        });
    }
    else
    {
        if(endCallback) endCallback(piecesToMove);
    }
}


void ColumnsGameController::UpdateBoardDestroyPieces(TilesSet piecesToDestroy)
{
    mColumnsBoardModel.RemovePieces(piecesToDestroy);

}
    
void ColumnsGameController::UpdateBoardMakePiecesFall(TilesMovementSet piecesToMove)
{
    mColumnsBoardModel.MovePieces(piecesToMove);
}

