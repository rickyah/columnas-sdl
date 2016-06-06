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
#include "ColumnsGameFSMDefinitions.hpp"

#include "ResourceManager.hpp"
#include "ColumnsGameStates.hpp"
#include "EventQueue.hpp"
#include "AppEvents.hpp"

#include "RandomDistribution.hpp"

#include "ColumnsBoard.hpp"
#include "ColumnsBoardView.hpp"

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
    
   
public:
    ColumnsGameController(EventQueue &eventQueue,
                          ResourceManager &resourceMng)
                            :
    mColumnsBoard(cDefaultBoardRows, cDefaultBoardColumns),
    mEventQueueRef(eventQueue),
    mResourceManagerRef(resourceMng)
    {}
    
    // TODO: use with a configuration POCO when we read configuration from files
    void Init();
    
    void EndGame();
    
    bool MovePlayerDown();
    void MovePlayerLeft();
    void MovePlayerRight();
    bool ResetPlayerBlock();
    void MovePlayerBlockPieces();
    
    void Update(TimeInfo time);
    void Render(TimeInfo time, std::shared_ptr<Renderer> pRenderer);

private:
    ColumnsGameController(const ColumnsGameController &);
    ColumnsGameController& operator=(const ColumnsGameController &);
    
    ColumnsGameFSM mFSM;
    
    // Creates a new player block with a random set of pieces and positions it at
    // the initial position in the board
    
    
    const uint8_t cDefaultBoardColumns = 17;
    const uint8_t cDefaultBoardRows = 6;
    
    ColumnsBoard mColumnsBoard;
    ColumnsBoardView mColumnsBoardView;

    std::shared_ptr<RandomDistribution> pRandomDistribution;
    
    ResourceManager &mResourceManagerRef;
    EventQueue &mEventQueueRef;
};

#endif /* ColumnsGameController_hpp */
