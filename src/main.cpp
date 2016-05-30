#include <algorithm>
#include "SDL.h"
#include "SDL_image.h"
#include "App.hpp"
#include "ColumnsBoardView.hpp"
#include "ColumnsBoard.hpp"
#include "crc32.hpp"

/*
 Produces a random int x, min <= x <= max
 following a uniform distribution
 */
int randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/*
 Produces a random float x, min <= x <= max
 following a uniform distribution
 */
float randomFloat(float min, float max)
{
    return rand() / (float) RAND_MAX *(max - min) + min;
}

int main(int argc, char *argv[])
{
    int done;

    TilePosition p1, p2;

    App app;

    ColumnsBoard board({
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,1,2,2,2,4},
        {2,2,1,2,1,1},
    });
    ColumnsBoardView view;
    
    
    board.SetPlayerBlockInitialPosition(TilePosition(0,2));
    
    board.ResetPlayerBlock({1,1,1});


    Position p = {10, 20};
    

    auto tex = app.graphics()->renderer()->LoadTextureFromFile("Sprites/Croissant@2x.png");

    auto tex2 = app.graphics()->renderer()->LoadTextureFromFile("Sprites/Cupcake@2x.png");
    view.SetPieceToTextureMapping(tex->realSize(),
                                  {
                                      {1, tex },
                                      {2, tex2 }
                                  });

    app.eventQueue()->AddListener(AppTouch_Event::sEventType, EventListenerDelegate([&board](std::shared_ptr<IEventData> data){
        auto ptr = std::static_pointer_cast<AppTouch_Event>(data);
        
        board.MovePlayerBlockDown();
    }));
    
    /* game loop */
    done = 0;
    
    app.SetLogicUpdateFunction([&view](App::TimeInfo time)  {
        SDL_Log("[%d] Game UPDATE [%f ms]\n", time.frameCount, time.dt);
    });
    
    app.SetRenderUpdateFunction([&app, &view, &board](App::TimeInfo time)  {
        view.SetBoardState(board.boardState());
        view.Render(app.graphics()->renderer());
        SDL_Log("[%d] RENDER [%f ms]\n", time.frameCount, time.dt);
    });
        
    app.UpdateGameLoop();

    return 0;
}
