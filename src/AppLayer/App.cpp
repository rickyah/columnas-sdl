//  CApp.cpp
//
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#include "App.hpp"
#include "SDLResourceLoaders.hpp"

#include <chrono>


App::App(int width, int height, bool fullscreen)
{
    // initialize SDL (each subsystem inits in the appropiated classes)
    if (SDL_Init(0)) {

        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    pWindow = std::make_shared<Window>(width, height, fullscreen);
    pEventQueue = std::make_shared<EventQueue>();
    pEventsManager = std::make_shared<EventsManager>(pWindow, pEventQueue);
    pResourceManager = std::make_shared<ResourceManager>(std::make_shared<SDLTextureLoader>(pWindow->pRenderer),
                                                         std::make_shared<SDLMixerSoundLoader>(),
                                                         std::make_shared<SDLMixerMusicLoader>());
    
    pAudio = std::make_shared<AudioPlayer>();
    
    // Make the app exit if the quit event is received
    pEventQueue->AddListener(AppQuit_Event::sEventType, [this](std::shared_ptr<IEventData>){
        this->Quit();
    });
}

App::~App()
{
    SDL_Quit();
}


double App::msSinceStart() const
{
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
}

void App::Delay(int32_t msecs) const
{
    SDL_Delay((Uint32)std::max(0, msecs));
}

void App::UpdateGameLoop()
{
    if (mShouldQuit) return;
    
    double previousFrameStartMs = 0;
    double currentFrameStartMs = msSinceStart();
    double accumulatorMs = 0.0;
    double previousFrameTime;
    
    // This returns false when the event to quit is detected
    while (pEventsManager->Update())
    {
        previousFrameStartMs = currentFrameStartMs;
        currentFrameStartMs = msSinceStart();
        
        // Clamp frametime max value
        previousFrameTime = std::min(300.0, (currentFrameStartMs - previousFrameStartMs));
        
//        printf("%f\n", previousFrameTime);
        // Simulate game
        accumulatorMs += previousFrameTime;
        while (accumulatorMs >= mLogicRateMs)
        {
            mLogicTimeInfo.dt = mLogicRateMs;
            mLogicTimeInfo.frameCount++;
            mLogicTimeInfo.elapsedMs = msSinceStart();
            
            if(mFuncLogicUpdate) mFuncLogicUpdate(mLogicTimeInfo);
            accumulatorMs -= mLogicRateMs;
        }

        // render frame
        mRenderTimeInfo.interpolation = accumulatorMs/mRenderRateMs;
        mRenderTimeInfo.frameCount++;
        mRenderTimeInfo.elapsedMs = msSinceStart();
        
        if(mFuncRenderUpdate) mFuncRenderUpdate(mRenderTimeInfo, pWindow->renderer());
        pWindow->pRenderer->Present();

        // todo: check this computation, something is odd because it introduces a slight input lag
        // The problem is that now the gameloob takes more CPU time
        /*
        double totalFrameTime =  msSinceStart() - previousFrameStartMs;
      
        if (totalFrameTime < mRenderRateMs)
        {
            printf("total:%f sleep: %f\n", totalFrameTime, mRenderRateMs - totalFrameTime);

            Delay(mRenderRateMs - totalFrameTime);
        }
        /*/
        Delay(1);
        //*/
        
    }
    
    mFuncRenderUpdate = nullptr;
    mFuncLogicUpdate = nullptr;
}
