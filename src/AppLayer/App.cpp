//  CApp.cpp
//
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "App.hpp"
#include "SDLResourceLoaders.hpp"

App::App()
{
    // initialize SDL (each subsystem inits in the appropiated classes)
    if (SDL_Init(0)) {

        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    _pGraphics = std::make_shared<Graphics>();
    _pEventQueue = std::make_shared<EventQueue>();
    _pEventsManager = std::make_shared<EventsManager>(_pEventQueue);
    _pResourceManager= std::make_shared<ResourceManager>(std::make_shared<SDLTextureLoader>());
}

App::~App()
{
    _pEventsManager.reset();
    _pEventQueue.reset();
    _pGraphics.reset();
    
    SDL_Quit();
}


uint32_t App::ticksSinceStart() const
{
    return SDL_GetTicks();
}

void App::Delay(int32_t msecs) const
{
    SDL_Delay((Uint32)std::max(0, msecs));
}

void App::UpdateGameLoop()
{
    double previousFrameStartTicks = ticksSinceStart();
    double accumulatorMs = 0.0;
    
    // This returns false when the event to quit is detected
    while (_pEventsManager->Update())
    {
        double currentFrameStartTicks = ticksSinceStart();
        // Clamp frametime max value
        double frameTimeMs = std::min(0.33, (currentFrameStartTicks - previousFrameStartTicks));
        previousFrameStartTicks = currentFrameStartTicks;
        
        accumulatorMs += frameTimeMs;
        
        // Simulate game
        while (accumulatorMs >= _logicRateMs )
        {
            _logicTimeInfo.dt = _logicRateMs;
            _logicTimeInfo.frameCount = _logicFrameCount++;
            _logicTimeInfo.elapsedMs = ticksSinceStart();
            
            if(_logicUpdateFc) _logicUpdateFc(_logicTimeInfo);
            accumulatorMs -= _logicRateMs;
        }

        // render

        _renderTimeInfo.dt = accumulatorMs/_renderRateMs;
        _renderTimeInfo.frameCount = _renderFrameCount++;
        _renderTimeInfo.elapsedMs = ticksSinceStart();
        
        if(_renderUpdateFc) _renderUpdateFc(_renderTimeInfo, graphics()->renderer());
        
        if (_renderRateMs > 0)
        {
            auto elapsedFrameMs =  ((ticksSinceStart() - previousFrameStartTicks));
            double sleepTimeMs = _renderRateMs - elapsedFrameMs;
        
            Delay(sleepTimeMs);
        }
    }
    
    return 0;
}