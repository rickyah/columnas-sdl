//
//  CApp.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "App.hpp"


App::App()
{
    // initialize SDL (each subsystem inits in the appropiated classes)
    if (SDL_Init(0)) {

        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    _pGraphics = std::make_shared<Graphics>();
    _pEventManager = std::make_shared<EventManager>();
}

App::~App()
{
    _pEventManager.reset();
    _pGraphics.reset();
    SDL_Quit();
}
