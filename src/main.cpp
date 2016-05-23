/*
 *  rectangles.c
 *  written by Holmes Futrell
 *  use however you want
*/

#include "SDL.h"
#include <time.h>
#include "common.h"
#include "CGraphics.hpp"
#include "EventManager.hpp"

class MyEvent : public IEventData<MyEvent>
{
public:
    MyEvent(): IEventData() {}
};

class MyEvent2 : public IEventData<MyEvent2>
{
public:
    MyEvent2(): IEventData() {}
};

template<> const EventType IEventData<MyEvent>::eventType = 10;
template<> const EventType IEventData<MyEvent2>::eventType = 20;


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


    CGraphics g(SCREEN_WIDTH, SCREEN_HEIGHT);

    MyEvent e1;
    MyEvent2 e2;
    
    auto t1 = e1.type();
    auto t2 = e2.type();
    
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
