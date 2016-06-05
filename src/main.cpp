#include <string>
#include "App.hpp"
#include "DataTypes.hpp"
#include "ColumnsGameController.hpp"

#include "ColumnsBoardView.hpp"
#include "ColumnsBoard.hpp"
#include "ResourcesDefinitions.hpp"

int main(int argc, char *argv[])
{
    TilePosition p1, p2;

    App app;

    
    auto tex1 = app.resourceManager()->Register<Texture2dResource>(RESOURCE_ID("Sprites/Croissant@2x.png"));
    auto tex2 = app.resourceManager()->Register<Texture2dResource>(RESOURCE_ID("Sprites/Cupcake@2x.png"));

    auto tex3 = app.resourceManager()->Register<Texture2dResource>(RESOURCE_ID("blau.png"));
    tex3->Load();
    
    ColumnsBoard board({
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,3},
        {0,0,0,0,3,3},
        {0,1,2,2,2,3},
        {2,2,1,2,1,1},
    });
    ColumnsBoardView view;
    
    
    board.InitPlayerBlockInitialPosition(TilePosition(0,2));
    
    board.ResetPlayerBlock({1,1,1});


    Position p = {10, 20};
    
    view.SetPieceToTextureMapping(tex1->texture()->drawSize(),
                                  {
                                      {1, tex1 },
                                      {2, tex2 },
                                      {3, tex3 }
                                  });

    app.eventQueue()->AddListener(AppTouch_Event::sEventType, EventListenerDelegate([&board](std::shared_ptr<IEventData> data){
        auto ptr = std::static_pointer_cast<AppTouch_Event>(data);
        
        board.MovePlayerBlockDown();
    }));
    
    /* game loop */
    
    app.SetLogicUpdateFunction([&view](TimeInfo time)  {
        SDL_Log("[%d] Game UPDATE [%f ms]\n", time.frameCount, time.dt);
    });
    
    app.SetRenderUpdateFunction([&app, &view, &board](TimeInfo time, std::shared_ptr<Renderer> renderer)  {
        view.SetBoardState(board.boardState());
        view.Render(renderer);
        SDL_Log("[%d] RENDER [%f ms]\n", time.frameCount, time.dt);
        
        renderer->Present();
    });
        
    app.UpdateGameLoop();

    return 0;
}
