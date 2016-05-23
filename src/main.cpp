/*
 *  rectangles.c
 *  written by Holmes Futrell
 *  use however you want
*/

#include "SDL.h"
#include <time.h>
#include "common.h"
#include "CGraphics.h"

void renderRandomColorRectangle(std::shared_ptr<CRenderer> renderer)
{

    Uint8 r, g, b;
    /*  Come up with a random rectangle */
    SDL_Rect rect;
    rect.w = randomInt(64, 128);
    rect.h = randomInt(64, 128);
    rect.x = randomInt(0, SCREEN_WIDTH);
    rect.y = randomInt(0, SCREEN_HEIGHT);

    /* Come up with a random color */
    r = randomInt(50, 255);
    g = randomInt(50, 255);
    b = randomInt(50, 255);

    /*  Fill the rectangle in the color */
    renderer->setColor(r, g, b, 255);
    renderer->fillRect(&rect);
    renderer->present();

}

int main(int argc, char *argv[])
{

    SDL_Window *window;
    SDL_Renderer *renderer;
    int done;
    SDL_Event event;

    
//    /* initialize SDL */
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        fatalError("Could not initialize SDL");
//    }

    CGraphics g(SCREEN_WIDTH, SCREEN_HEIGHT);

    
    g.renderer()->clear();
    

    /* Enter render loop, waiting for user to quit */
    done = 0;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_FINGERDOWN) {
                done = 1;
            }
            
            SDL_Log("Polled event %d", event.type);
        }
        renderRandomColorRectangle(g.renderer());
        SDL_Delay(1);
    }

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}
