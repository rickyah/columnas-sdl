//
//  Tween.hpp
//
//
//  Created by Ricardo Amores Hernández on 12/6/16.
//
//

#ifndef Tween_hpp
#define Tween_hpp
#include <functional>
#include <cmath>

#include "TweenerFunctions.hpp"

class Tween
{
public:
    
    enum class Type {
        
        // Plays once and stops. Default
        Once,
        
        // Plays the animation backward and stops.
        Reverse,

        // Plays the animation forward and then plays it backwards and then stops. The animation's durations is
        // effectibly doubled in this case.
        Pingpong,
        
        // Restarts the animation immediately when it finishes, the same way if you have called Reset() method.
        // This means the animation never stops unless you stop it manually.
        Loop,
        // This is a combination of PingPong and Loop, so the animation goes forward and backward indefinitely
        
        // until you stop it manually
        PingpongLoop
        
        
    };
    
    typedef std::function<float(float t, float b, float c, float d)> EasingFunction;
    typedef std::function<void(const Tween& tween)> EndCallback;
    
    Tween();
    Tween(float initialValue, float endValue, float durationMs, EasingFunction func);  
    Tween(const Tween &ori);
    Tween(Tween &&ori);
    ~Tween();
    Tween& operator=(Tween other);
    Tween& operator=(Tween &&other);
    
    // Starts the tween. If the tween is not started calls to Update methods does not take effect
    Tween& Start();
    
    // Stops the tween. An stopped tween keeps the state it had when stop was called but can't be resumed, you need to
    // call Start to restart the Tween.
    Tween& Stop();
    
    // Pauses the execution of a running Tween, meaning that the Update() calls does not take effect.
    // Execution can be resumed calling the Resume() method.
    Tween& Pause();
    
    // Resumes the execution of a paused Tween. Has no effect on stopped or not started Tweens
    Tween& Resume();
    
    
    
    // Query state
    float initialValue() const { return mInitialValue; }
    float endValue() const { return mEndValue; }
    float currentValue() const { return mValueSinceLastUpdate; }
    float initialDelayMs() const { return mInitialDelayMs; }
    float durationMs() const { return mDurationMs; }
    Type type() const { return mTweenType; }
    bool hasStarted() const { return mHasStarted; }
    bool isRunning() const;
    bool hasFinished() const;
    
    // Change state. Only if the Tween has not started yet or it is stopped before changing, otherwise
    // calls to these methods have no effect
    Tween& initialValue(float newValue);
    Tween& initialDelayMs(float newValue);
    Tween& endValue(float newValue);
    Tween& durationMs(float newValue);
    Tween& type(Type newType);
    Tween& endCallback(EndCallback func);
    Tween& easingFunction(EasingFunction func);
    
    
    float speedMultiplier() const { return mSpeedMultiplier; }
    Tween& speedMultiplier(float newSpeed) { mSpeedMultiplier = std::fmax(0, newSpeed); return *this;}

    void Update(float dt);
    
private:
    
    // Stops the tween and sets back the original values
    Tween& Reset();
    
    friend void swap(Tween& first, Tween& second) // nothrow
    {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        
        swap(first.mHasStarted, second.mHasStarted);
        swap(first.mIsPaused, second.mIsPaused);
        swap(first.mTweenType, second.mTweenType);
        swap(first.mSpeedMultiplier, second.mSpeedMultiplier);
        swap(first.mElapsedTimeMs, second.mElapsedTimeMs);
        swap(first.mDurationMs, second.mDurationMs);
        swap(first.mValueSinceLastUpdate, second.mValueSinceLastUpdate);
        swap(first.mInitialValue, second.mInitialValue);
        swap(first.mEndValue, second.mEndValue);
        swap(first.mInitialValueInUpdate, second.mInitialValueInUpdate);
        swap(first.mEndValueInUpdate, second.mEndValueInUpdate);
        swap(first.mIsPingPongAnimationBackwards, second.mIsPingPongAnimationBackwards);
        swap(first.mElapsedInitialDelayMs, second.mElapsedInitialDelayMs);
        swap(first.mEndCallbackFunc, second.mEndCallbackFunc);
        swap(first.mEasingFunc, second.mEasingFunc);
        swap(first.mWasEndCallbackFuncCalled, second.mWasEndCallbackFuncCalled);
    }
    
    bool mHasStarted = false;
    bool mIsPaused = false;
    bool mWasEndCallbackFuncCalled = false;
    Tween::Type mTweenType;
    
    float mInitialDelayMs = 0;
    float mSpeedMultiplier = 1.0;
    float mElapsedTimeMs;
    float mValueSinceLastUpdate;
    float mInitialValue;
    float mEndValue;
    float mDurationMs;
    
    // These are the values used in the interpolation as the animations could be reversed or
    // the duration modified
    float mInitialValueInUpdate;
    float mEndValueInUpdate;
    
    bool mIsPingPongAnimationBackwards;
    
    float mElapsedInitialDelayMs;
    
    EndCallback mEndCallbackFunc;
    EasingFunction mEasingFunc;
};

#endif /* Tween_hpp */
