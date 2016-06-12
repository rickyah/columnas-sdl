//
//  Tween.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 12/6/16.
//
//

#include "Tween.hpp"

Tween::Tween(float initialValue, float endValue, float durationMs, EasingFunction func):
    mInitialValue(initialValue),
    mEndValue(endValue),
    mDurationMs(durationMs),
    mEasingFunc(func),
    mValueSinceLastUpdate(initialValue),
    mElapsedTimeMs(0)
{
    
}

void Tween::Update(float dt)
{
    mElapsedTimeMs += dt;
    mValueSinceLastUpdate = mEasingFunc(mElapsedTimeMs, mInitialValue, mEndValue - mInitialValue, mDurationMs);
    
    mValueSinceLastUpdate = std::fmin(mValueSinceLastUpdate, mEndValue);
}

void Tween::Restart()
{
    Restart(mInitialValue, mEndValue, mDurationMs);
}

void Tween::Restart(float initialValue, float endValue, float durationMs)
{
    mElapsedTimeMs = 0;
    mInitialValue = initialValue;
    mEndValue = endValue;
    mDurationMs = durationMs;
    mValueSinceLastUpdate = mInitialValue;
}