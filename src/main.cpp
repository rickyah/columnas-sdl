#include "SDL.h"
#include "SDL_image.h"
#include "App.hpp"
#include "CColumnsBoardView.hpp"

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
    SDL_Event event;

    App app;

    CColumnsBoardView view;

    Position p = {10, 20};

    view.SetBoardState({
        {0,0,0},
        {0,1,0},
        {1,1,1}
    });

    auto tex = g.renderer().lock()->LoadTextureFromFile("Sprites/Croissant.png");

    view.SetPieceToTextureMapping(tex->realSize(),
                                  {
                                      {1, tex }
                                  });


//    auto pCroissantTex = g.renderer()->LoadTextureFromFile("Sprites/Croissant.png");
//    g.renderer()->SetColor(128, 128, 128);
//    g.renderer()->Clear();
    
    /* Enter render loop, waiting for user to quit */
    done = 0;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_FINGERDOWN) {
                //app.graphics()->renderer()->DrawTextureAt(pCroissantTex, {static_cast<int>(app.graphics()->width() * event.tfinger.x), static_cast<int>(app.graphics()->height() * event.tfinger.y)});

            }
            if (event.type == SDL_QUIT) {
                done = 1;
            }
                
            view.Render(g.renderer());
            SDL_Log("Polled event %d", event.type);
        }
        
        SDL_Delay(1);
        app.graphics()->renderer()->Present();

    }

    return 0;
}
