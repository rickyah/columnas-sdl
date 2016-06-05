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

#include "ColumnsBoard.hpp"
#include "ColumnsBoardView.hpp"
#include "ResourceManager.hpp"
#include "EventQueue.hpp"

class ColumnsGameController
{
    enum EPieces
    {
        Tile        = 0,
        Croissant   = 1,
        Cupcake     = 2,
        Danish      = 3,
        Donut       = 4,
        Macaroon    = 5,
        Cookie      = 6
    };
    
public:
    ColumnsGameController(std::shared_ptr<EventQueue> eventQueue,
                          std::shared_ptr<ResourceManager> resourceMng)
                            :
    mColumnsBoard(cDefaultBoardState),
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
    
    const BoardState cDefaultBoardState = {
        // Space allowing pieces to be put on the board
        // if the game finish condition is met
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        // Actual game board
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0}
    };
    ColumnsBoard mColumnsBoard;
    ColumnsBoardView mColumnsBoardView;

    std::shared_ptr<ResourceManager> pResourceManager;
    std::shared_ptr<EventQueue> pEventQueue;
};

#endif /* ColumnsGameController_hpp */
