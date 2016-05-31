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
    
    struct TimeInfo
    {
        double dt;
        int frameCount;
        int elapsedMs;
    };
    
    App();
    ~App();
    
    void UpdateGameLoop();
    
    // Returns a reference to the graphics subsystem
    std::shared_ptr<Graphics> graphics() { return _pGraphics; }
    
    // Gets a reference to the global EventQueue to dispatch messages between systems
    std::shared_ptr<EventQueue> eventQueue() { return _pEventQueue; }
    
    // Gets a reference to the system's event processor
    std::shared_ptr<EventsManager> eventsManager() { return _pEventsManager; }
    
    // Gets a reference to the system's resource manager
    std::shared_ptr<ResourceManager> resourceManager() { return _pResourceManager; }
    
    Uint32 ticksSinceStart() const;
    
    void Delay(int32_t msecs) const;
    /*
     * This sets the std::function to call on each logic update. 
     * 
     * The function receives the following parameters
     * @param double delta time (in this case should be a fixed value)
     * @param int frameCount
     */
    void SetLogicUpdateFunction(std::function<void (TimeInfo)> func) { _logicUpdateFc = func; }
    
    /*
     * This sets the std::function to call on each render update.
     *
     * The function receives the following parameters
     * @param double delta time
     * @param int frameCount
     */
    void SetRenderUpdateFunction(std::function<void (TimeInfo)> func) { _renderUpdateFc = func; }
    
    /*
     * Get the update rate for the logic calls. Defaults to 120 per second
     */
    double logicFPS() const { return _logicRateMs * 1000.0; }
    
    /*
     * Sets the update rate for the logic calls in miliseconds.
     */
    void logicFPS(double fps) { _logicRateMs = 1000.0/fps; }
    
    /*
     * Get the update rate for the render calls. Defaults to 1000/60 (60 per second)
     */
    double renderFPS() const { return _renderRateMs*1000.0; }
    
    /*
     * Sets the update rate for the logic calls in miliseconds.
     */
    void setRenderFPS(double fps) { _renderRateMs = 1000.0/fps; }
    
    // Count of current number of logic updates since the app started
    int logicFrameCount() const { return _logicFrameCount; }
    
    // Count of current number of render updates since the app started
    int renderFrameCount() const {return _renderFrameCount; }
    
private:
    double _logicRateMs = 1000/120.0;
    double _renderRateMs = 1000/60.0;
    int _logicFrameCount = 0;
    int _renderFrameCount = 0;
    
    std::shared_ptr<Graphics> _pGraphics;
    std::shared_ptr<EventQueue> _pEventQueue;
    std::shared_ptr<EventsManager> _pEventsManager;
    std::shared_ptr<ResourceManager> _pResourceManager;
    
    std::function<void (TimeInfo)> _logicUpdateFc;
    TimeInfo _logicTimeInfo;
    std::function<void (TimeInfo)> _renderUpdateFc;
    TimeInfo _renderTimeInfo;
};

#endif /* App_hpp */
