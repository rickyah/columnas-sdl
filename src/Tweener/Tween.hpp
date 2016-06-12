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
    bool isFinished() const { return mElapsedTimeMs >= mDurationMs; }
    void Update(float dt);
    
    void Restart();
    void Restart(float initialValue, float endValue, float durationMs);
    
private:
    Tween();
    Tween(const Tween &);
    Tween& operator=(const Tween &);
    
    float mElapsedTimeMs;
    float mValueSinceLastUpdate;
    float mInitialValue;
    float mEndValue;
    float mDurationMs;
    
    EasingFunction mEasingFunc;
};

#endif /* Tween_hpp */
