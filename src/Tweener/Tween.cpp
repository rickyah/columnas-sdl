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
    mDurationMs(std::fmax(0.0, durationMs)),
    mEasingFunc(func),
    mValueSinceLastUpdate(initialValue),
    mElapsedTimeMs(0)
{
    
}

void Tween::Update(float dt)
{
    if (!mHasStarted) return;
    
    mElapsedTimeMs += dt * mSpeedMultiplier;
    mValueSinceLastUpdate = mEasingFunc(mElapsedTimeMs, mInitialValue, mEndValue - mInitialValue, mDurationMs);
    
    mValueSinceLastUpdate = std::fmin(mValueSinceLastUpdate, mEndValue);
}

void Tween::Restart()
{
    mValueSinceLastUpdate = mInitialValue;
    mElapsedTimeMs = 0;
    Start();
}

void Tween::Reset()
{
    Reset(mInitialValue, mEndValue, mDurationMs);
}

void Tween::Reset(float initialValue, float endValue, float durationMs)
{
    mElapsedTimeMs = 0;
    mInitialValue = initialValue;
    mEndValue = endValue;
    mDurationMs = std::fmax(0.0, durationMs);
    mValueSinceLastUpdate = mInitialValue;
    Stop();
}