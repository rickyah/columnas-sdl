//
//  Tween.hpp
//  Columns
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
    typedef std::function<float(float t, float b, float c, float d)> EasingFunction;
    
    Tween(float initialValue, float endValue, float durationMs, EasingFunction func);
    
    float initialValue() const { return mInitialValue; }
    float endValue() const { return mEndValue; }
    float currentValue() const { return mValueSinceLastUpdate; }
    float durationMs() const { return mDurationMs; }
    bool isRunning() const { return hasStarted() && !hasFinished(); }
    bool hasStarted() const { return mHasStarted; }
    bool hasFinished() const { return mElapsedTimeMs >= mDurationMs; }
    
    float speedMultiplier() const { return mSpeedMultiplier; }
    void speedMultiplier(float newSpeed) { mSpeedMultiplier = std::fmax(0, newSpeed); }
    void Start() { mHasStarted = true; }
    void Stop() { mHasStarted = false; }
    void Restart();
    void Update(float dt);
    void Reset();
    void Reset(float initialValue, float endValue, float durationMs);
    
private:
    Tween();
    Tween(const Tween &);
    Tween& operator=(const Tween &);
    
    bool mHasStarted = false;
    
    float mSpeedMultiplier = 1.0;
    float mElapsedTimeMs;
    float mValueSinceLastUpdate;
    float mInitialValue;
    float mEndValue;
    float mDurationMs;
    
    EasingFunction mEasingFunc;
};

#endif /* Tween_hpp */
