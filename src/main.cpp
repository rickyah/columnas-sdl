/*
 *  rectangles.c
 *  written by Holmes Futrell
 *  use however you want
*/

#include "SDL.h"
#include "SDL_image.h"
#include <time.h>
#include "CGraphics.hpp"
#include "EventManager.hpp"

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

void fatalError(const char *string)
{
    printf("%s: %s\n", string, SDL_GetError());
    exit(1);
}


int main(int argc, char *argv[])
{
    int done;
    SDL_Event event;

    // initialize SDL (each subsystem inits in the appropiated classes)
    if (SDL_Init(0)) {
        fatalError("Could not initialize SDL");
    }

    CGraphics g;

    auto pCroissantTex = g.renderer()->LoadTextureFromFile("Sprites/Croissant.png");
    g.renderer()->SetColor(128, 128, 128);
    g.renderer()->Clear();
    
    
    /* Enter render loop, waiting for user to quit */
    done = 0;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_FINGERDOWN) {
                g.renderer()->DrawTextureAt(pCroissantTex, {static_cast<int>(g.width() * event.tfinger.x), static_cast<int>(g.height() * event.tfinger.y)});
            }
            if (event.type == SDL_QUIT) {
                done = 1;
            }
                
            
            SDL_Log("Polled event %d", event.type);
        }
        
        SDL_Delay(1);
        g.renderer()->Present();
    }

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}
