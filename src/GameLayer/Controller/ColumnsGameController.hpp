//
//  ColumnsGameController.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#ifndef ColumnsGameController_hpp
#define ColumnsGameController_hpp

#include <memory>

#include "FSM.hpp"
#include "ResourceManager.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

#include "RandomDistribution.hpp"
#include "ColumnsBoard.hpp"
#include "ColumnsBoardView.hpp"


class ColumnsGameState : IState
{
protected:
    ColumnsGameState() {}
    
private:
    ColumnsGameState(const ColumnsGameState &);
    ColumnsGameState & operator=(const ColumnsGameState &);
};

class MovingPiecesState : ColumnsGameState
{
public:
    MovingPiecesState(const EventQueue & eventQueue);
    
private:
    
};

class RemovingPiecesState : ColumnsGameState
{
public:
    
};

class DroppingPiecesState : ColumnsGameState
{
public:
    DroppingPiecesState();
    
    DroppingPiecesState(const DroppingPiecesState &) = delete;
    DroppingPiecesState & operator=(const DroppingPiecesState &) = delete;
};

class ColumnsGameController
{
    enum EPieces
    {
        Croissant   = 1,
        Cupcake     = 2,
        Danish      = 3,
        Donut       = 4,
        Macaroon    = 5,
        Cookie      = 6
    };
    
    enum EStates
    {
        Moving_Pieces,
        Removing_Pieces,
        Dropping_Pieces
    };
    
public:
    ColumnsGameController(std::shared_ptr<EventQueue> eventQueue,
                          std::shared_ptr<ResourceManager> resourceMng)
                            :
    mColumnsBoard(cDefaultBoardWidth, cDefaultBoardHeight),
    pEventQueue(eventQueue),
    pResourceManager(resourceMng)
    {}
    
    // TODO: use with a configuration POCO when we read configuration from files
    void Init();
    
    
    void Update(TimeInfo time);
    void Render(TimeInfo time, std::shared_ptr<Renderer> pRenderer);

private:
    ColumnsGameController(const ColumnsGameController &);
    ColumnsGameController& operator=(const ColumnsGameController &);
    
    
    FSM<EStates, IState, EnumHasher> mFSM;
    
    // Creates a new player block with a random set of pieces and positions it at
    // the initial position in the board
    bool ResetPlayerBlock();
    void MovePlayerBlockPieces();
    
    const uint8_t cDefaultBoardWidth = 6;
    const uint8_t cDefaultBoardHeight = 17;
    
    ColumnsBoard mColumnsBoard;
    ColumnsBoardView mColumnsBoardView;

    std::shared_ptr<RandomDistribution> pRandomDistribution;
    
    std::shared_ptr<ResourceManager> pResourceManager;
    std::shared_ptr<EventQueue> pEventQueue;
};

#endif /* ColumnsGameController_hpp */
