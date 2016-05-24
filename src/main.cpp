/*
 *  rectangles.c
 *  written by Holmes Futrell
 *  use however you want
*/

#include "SDL.h"
#include <time.h>
#include "CGraphics.hpp"
#include "EventManager.hpp"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 920


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


void renderRandomColorRectangle(std::shared_ptr<CRenderer> renderer)
{
    /*  Fill the rectangle in the color */
    renderer->setColor(randomInt(50, 255),
                       randomInt(50, 255),
                       randomInt(50, 255),
                       randomInt(0, 10));
    
    renderer->fillRectangle(randomInt(0, SCREEN_WIDTH),
                            randomInt(0, SCREEN_WIDTH),
                            randomInt(0, SCREEN_WIDTH),
                            randomInt(0, SCREEN_HEIGHT));
    
    renderer->present();

}

int main(int argc, char *argv[])
{
    int done;
    SDL_Event event;

//    /* initialize SDL */
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        fatalError("Could not initialize SDL");
//    }
    
    CGraphics g;
    
    g.renderer()->clear();

    /* Enter render loop, waiting for user to quit */
    renderRandomColorRectangle(g.renderer());
    done = 0;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_FINGERDOWN) {
                renderRandomColorRectangle(g.renderer());
            }
            if (event.type == SDL_QUIT) {
                done = 1;
            }
                
            
            SDL_Log("Polled event %d", event.type);
        }
        
        SDL_Delay(1);
    }

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}
