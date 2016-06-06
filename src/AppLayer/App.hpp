//
//  CApp.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 26/5/16.
//
//

#ifndef App_hpp
#define App_hpp

#include <memory>
#include "SDL.h"
#include "DataTypes.hpp"
#include "Graphics.hpp"
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
    
    App();
    ~App();
    
    void UpdateGameLoop();
    
    // Returns a reference to the graphics subsystem
    std::shared_ptr<Graphics> graphics() { return pGraphics; }
    
    // Gets a reference to the global EventQueue to dispatch messages between systems
    std::shared_ptr<EventQueue> eventQueue() { return pEventQueue; }
    
    // Gets a reference to the system's event processor
    std::shared_ptr<EventsManager> eventsManager() { return pEventsManager; }
    
    // Gets a reference to the system's resource manager
    std::shared_ptr<ResourceManager> resourceManager() { return pResourceManager; }
    
    Uint32 ticksSinceStart() const;
    
    void Delay(int32_t msecs) const;
    /*
     * This sets the std::function to call on each logic update. 
     * 
     * The function receives the following parameters
     * @param double delta time (in this case should be a fixed value)
     * @param int frameCount
     */
    void SetLogicUpdateFunction(std::function<void (TimeInfo)> func) { mFuncLogicUpdate = func; }
    
    /*
     * This sets the std::function to call on each render update.
     *
     * The function receives the following parameters
     * @param double delta time
     * @param int frameCount
     */
    void SetRenderUpdateFunction(std::function<void (TimeInfo, std::shared_ptr<Renderer>)> func) { mFuncRenderUpdate = func; }
    
    /*
     * Get the update rate for the logic calls. Defaults to 120 per second
     */
    double logicFPS() const { return mLogicRateMs * 1000.0; }
    
    /*
     * Sets the update rate for the logic calls in miliseconds.
     */
    void logicFPS(double fps) { mLogicRateMs = fps <= 0? UpdateRate::FPS120 : (1000.0/fps); }
    
    /*
     * Get the update rate for the render calls. Defaults to 1000/60 (60 per second)
     */
    double renderFPS() const { return mRenderRateMs * 1000.0; }
    
    /*
     * Sets the update rate for the logic calls in miliseconds.
     */
    void renderFPS(double fps) { mRenderRateMs = fps <=0 ? UpdateRate::FPSUncapped : (1000.0/fps); }
    
    // Count of current number of logic updates since the app started
    int logicFrameCount() const { return mLogicFrameCount; }
    
    // Count of current number of render updates since the app started
    int renderFrameCount() const {return mRenderFrameCount; }
    
private:
   
    double mLogicRateMs = 1000/UpdateRate::FPS120;
    double mRenderRateMs = 1000/UpdateRate::FPS60;
    int mLogicFrameCount = 0;
    int mRenderFrameCount = 0;
    
    std::shared_ptr<Graphics> pGraphics;
    std::shared_ptr<EventQueue> pEventQueue;
    std::shared_ptr<EventsManager> pEventsManager;
    std::shared_ptr<ResourceManager> pResourceManager;
    
    TimeInfo mLogicTimeInfo;
    TimeInfo mRenderTimeInfo;
    std::function<void (TimeInfo)> mFuncLogicUpdate;
    std::function<void (TimeInfo, std::shared_ptr<Renderer>)> mFuncRenderUpdate;
};

#endif /* App_hpp */
