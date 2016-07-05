//
//  CApp.hpp
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef App_hpp
#define App_hpp

#include <memory>

#include <SDL2/SDL.h>

#include "DataTypes.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "AudioPlayer.hpp"
#include "EventQueue.hpp"
#include "EventsManager.hpp"
#include "ResourceManager.hpp"
#include "ResourceLoaderInterfaces.hpp"

/*
 * Application layer wrapper
 * This class will be used to abstract the different platforms under a common API
 *
 * Includes an implementation of gameloop using a fixed step to update the logic and a variable
 * step to render.
 *
 * @see UpdateGameLoop();
 * Logic update and render update are separated in the gameloop
 * @see logicRateMs() to get/set the logic rate
 * @see renderRateMs() to get/set the render rate
 * @see SetLogicUpdateFunction to set the function to call in each logic step
 * @see SetRenderUpdateFunction to set the function to call in each render step
 */
class App
{
public:
    
    enum UpdateRate
    {
        FPSUncapped = 0,
        FPS30 = 30,
        FPS60 = 60,
        FPS120 = 120
    };
    
    App(): App(0,0, false){};
    App(int width, int height, bool fullscreen);
    ~App();
    
    void UpdateGameLoop();
    
    // Returns a reference to the main window
    Window & window() { return *pWindow; }
    
    AudioPlayer & audio() { return *pAudio; }
    
    // Gets a reference to the global EventQueue to dispatch messages between systems
    EventQueue & eventQueue() { return *pEventQueue; }
    
    // Gets a reference to the system's event processor
    EventsManager & eventsManager() { return *pEventsManager; }
    
    // Gets a reference to the system's resource manager
    ResourceManager &resourceManager() { return *pResourceManager; }
    
    void Quit() { mShouldQuit = true; }
    
    double msSinceStart() const;
    
    void Delay(int32_t msecs) const;
    /*
     * This sets the std::function to call on each logic update. 
     * 
     * The function receives the following parameters
     * @param float delta time (in this case should be a fixed value)
     * @param int frameCount
     */
    void SetLogicUpdateFunction(std::function<void (LogicFrameInfo)> func) { mFuncLogicUpdate = func; }
    
    /*
     * This sets the std::function to call on each render update.
     *
     * The function receives the following parameters
     * @param float delta time
     * @param int frameCount
     */
    void SetRenderUpdateFunction(std::function<void (RenderFrameInfo, Renderer&)> func) { mFuncRenderUpdate = func; }
    
    /*
     * Get the update rate for the logic calls. Defaults to 120 per second
     */
    float logicFPS() const { return mLogicRateMs * 1000.0; }
    
    /*
     * Sets the update rate for the logic calls in miliseconds.
     */
    void logicFPS(float fps) { mLogicRateMs = fps <= 0? UpdateRate::FPS120 : (1000.0/fps); }
    
    /*
     * Get the update rate for the render calls. Defaults to 1000/60 (60 per second)
     */
    float renderFPS() const { return mRenderRateMs * 1000.0; }
    
    /*
     * Sets the update rate for the logic calls in miliseconds.
     */
    void renderFPS(float fps) { mRenderRateMs = fps <=0 ? UpdateRate::FPSUncapped : (1000.0/fps); }
    
    // Count of current number of logic updates since the app started
    int logicFrameCount() const { return mLogicTimeInfo.frameCount; }
    
    // Count of current number of render updates since the app started
    int renderFrameCount() const {return mRenderTimeInfo.frameCount; }
    
private:
   
    bool mShouldQuit = false;
    
    float mLogicRateMs = 1000/UpdateRate::FPS120;
    float mRenderRateMs = 1000/UpdateRate::FPS60;
    
    std::shared_ptr<Window> pWindow;
    std::shared_ptr<AudioPlayer> pAudio;
    std::shared_ptr<EventQueue> pEventQueue;
    std::shared_ptr<EventsManager> pEventsManager;
    std::shared_ptr<ResourceManager> pResourceManager;
    
    LogicFrameInfo mLogicTimeInfo;
    RenderFrameInfo mRenderTimeInfo;
    std::function<void (LogicFrameInfo)> mFuncLogicUpdate;
    std::function<void (RenderFrameInfo, Renderer &)> mFuncRenderUpdate;
};

#endif /* App_hpp */
