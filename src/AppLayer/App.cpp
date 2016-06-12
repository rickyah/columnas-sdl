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
    
    pGraphics = std::make_shared<Graphics>();
    pEventQueue = std::make_shared<EventQueue>();
    pEventsManager = std::make_shared<EventsManager>(pGraphics, pEventQueue);
    pResourceManager = std::make_shared<ResourceManager>(std::make_shared<SDLTextureLoader>());
}

App::~App()
{
    pResourceManager.reset();
    pEventsManager.reset();
    pEventQueue.reset();
    pGraphics.reset();
    
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
    while (pEventsManager->Update())
    {
        double currentFrameStartTicks = ticksSinceStart();
        
        // Clamp frametime max value
        double frameTimeMs = std::min(30.0, (currentFrameStartTicks - previousFrameStartTicks));
        previousFrameStartTicks = currentFrameStartTicks;
        
        accumulatorMs += frameTimeMs;
        
        // Simulate game
        while (accumulatorMs >= mLogicRateMs)
        {
            mLogicTimeInfo.dt = mLogicRateMs;
            mLogicTimeInfo.frameCount++;
            mLogicTimeInfo.elapsedMs = ticksSinceStart();
            
            if(mFuncLogicUpdate) mFuncLogicUpdate(mLogicTimeInfo);
            accumulatorMs -= mLogicRateMs;
        }


        // render frame
        mRenderTimeInfo.interpolation = accumulatorMs/mRenderRateMs;
        mRenderTimeInfo.frameCount++;
        mRenderTimeInfo.elapsedMs = ticksSinceStart();
        
        if(mFuncRenderUpdate) mFuncRenderUpdate(mRenderTimeInfo, graphics().renderer());
        
        if (mRenderRateMs > 0)
        {
            auto elapsedFrameSecs =  ((ticksSinceStart() - previousFrameStartTicks));
            double sleepTimeMs = mRenderRateMs - elapsedFrameSecs;
        
            Delay(sleepTimeMs);
        }
    }
    
    return 0;
}