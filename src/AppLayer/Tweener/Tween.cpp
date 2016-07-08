//
//  Tween.cpp
//
//
//  Created by Ricardo Amores Hernández on 12/6/16.
//
//

#include "Tween.hpp"

Tween::Tween():Tween(0, 0 ,0, Linear::easeIn){}

Tween::Tween(float initialValue, float endValue, float durationMs, EasingFunction func):
    mInitialDelayMs(0.0),
    mInitialValue(initialValue),
    mEndValue(endValue),
    mDurationMs(std::fmax(0.0, durationMs)),
    mEasingFunc(func),
    mValueSinceLastUpdate(initialValue),
    mElapsedTimeMs(0),
    mElapsedInitialDelayMs(0),
    mTweenType(Tween::Type::Once),
    mEndCallbackFunc(nullptr)
{
    
}

Tween::Tween(const Tween &ori)
{
    mHasStarted = ori.mHasStarted;
    mIsPaused = ori.mIsPaused;
    mTweenType = ori.mTweenType;
    
    mInitialValue = ori.mInitialValue;
    mEndValue = ori.mEndValue;
    mDurationMs = ori.mDurationMs;
    mEasingFunc = ori.mEasingFunc;
    
    mSpeedMultiplier = ori.mSpeedMultiplier;
    mElapsedTimeMs = ori.mElapsedTimeMs;
    mValueSinceLastUpdate = ori.mValueSinceLastUpdate;
    
    mInitialValueInUpdate = ori.mInitialValueInUpdate;
    mEndValueInUpdate = ori.mEndValueInUpdate;
    mIsPingPongAnimationBackwards = ori.mIsPingPongAnimationBackwards;
    mElapsedInitialDelayMs = ori.mElapsedInitialDelayMs;
    mWasEndCallbackFuncCalled = ori.mWasEndCallbackFuncCalled;
    
    
    mEndCallbackFunc = ori.mEndCallbackFunc;
    
}
Tween::~Tween()
{
    mEndCallbackFunc = nullptr;
    mEasingFunc = nullptr;
}

Tween::Tween(Tween &&other):Tween()
{
    swap(*this, other);
}

Tween& Tween::operator=(Tween other)
{
    swap(*this, other);
    
    return *this;
}

Tween& Tween::operator=(Tween &&other)
{
    return *this;
}


bool Tween::hasFinished() const
{
    if (mTweenType != Tween::Type::Pingpong)
    {
        return mElapsedTimeMs >= mDurationMs;
    }
    
    return mElapsedTimeMs >= mDurationMs *2;
}

bool Tween::isRunning() const
{
    return hasStarted() && !hasFinished() && !mIsPaused;
}

Tween& Tween::Start()
{
    if(!hasStarted())
    {
        Reset();
        
        mHasStarted = true;
    }
    
    return *this;
}

Tween& Tween::Restart()
{
    Stop();
    Start();
    
    return *this;
}

Tween& Tween::Resume()
{
    mIsPaused = false;
    
    return *this;
}

Tween& Tween::Pause()
{
    mIsPaused = true;
    
    return *this;
}


Tween& Tween::Stop()
{
    mHasStarted = false;
    
    return *this;
}

Tween& Tween::Reset()
{
    Stop();
    
    mHasStarted = mIsPaused = mWasEndCallbackFuncCalled = false;
    
    mValueSinceLastUpdate = mInitialValue;
    mElapsedTimeMs = mElapsedInitialDelayMs = 0;
    
    mInitialValueInUpdate = mInitialValue;
    mEndValueInUpdate = mEndValue;
    
    if (mTweenType == Tween::Type::Reverse)
    {
        mInitialValueInUpdate = mEndValue;
        mEndValueInUpdate = mInitialValue;
        mValueSinceLastUpdate = mEndValue;
    }
    
    mIsPingPongAnimationBackwards = false;

    
    return *this;
}

Tween& Tween::initialValue(float newValue)
{
    if(!hasStarted())
    {
        mInitialValue = newValue;
    }

    return *this;
}

Tween& Tween::endValue(float newValue)
{
    if(!hasStarted())
    {
        mEndValue = newValue;
    }
    
    return *this;
}

Tween& Tween::initialDelayMs(float newValue)
{
    if (!hasStarted())
    {
        mInitialDelayMs = newValue;
    }
    
    return *this;
}


Tween& Tween::durationMs(float newValue)
{
    if(!hasStarted())
    {
        mDurationMs = newValue;
    }
    
    return *this;
}

Tween& Tween::type(Type newType)
{
    if(!hasStarted())
    {
        mTweenType = newType;
    }
    
    return *this;
}

Tween& Tween::endCallback(EndCallback func)
{
    if (!hasStarted())
    {
        mEndCallbackFunc = func;
    }
    
    return *this;
}

Tween& Tween::easingFunction(EasingFunction func)
{
    if (!hasStarted())
    {
        mEasingFunc = func;
    }
    
    return *this;
}

void Tween::Update(float dt)
{
    if (!mHasStarted || mIsPaused) return;
    
    dt *= mSpeedMultiplier;
    
    // Do not update until we overpassed the initial delay
    if(mElapsedInitialDelayMs < mInitialDelayMs)
    {
        mElapsedInitialDelayMs += dt;
        
        // We overpass the initial delay, get the time we passed so it is the new dt
        if (mElapsedInitialDelayMs >= mInitialDelayMs)
        {
            dt = mElapsedInitialDelayMs - mInitialDelayMs;
        }
        else
        {
            return;
        }
    }
    
    mElapsedTimeMs += dt;
    
    if (type() == Tween::Type::Pingpong)
    {
        // forward
        if (mElapsedTimeMs <= mDurationMs)
        {
            mValueSinceLastUpdate = mEasingFunc(mElapsedTimeMs, mInitialValue, mEndValue - mInitialValue, mDurationMs);
        }
        // backwards
        else if (mElapsedTimeMs <= mDurationMs*2)
        {
            if(!mIsPingPongAnimationBackwards)
            {
                std::swap(mInitialValueInUpdate, mEndValueInUpdate);
                mIsPingPongAnimationBackwards = true;
            }
            mValueSinceLastUpdate = mEasingFunc(std::fmod(mElapsedTimeMs, mDurationMs), mInitialValueInUpdate, mEndValueInUpdate - mInitialValueInUpdate, mDurationMs);
        }
        else
        {
            mValueSinceLastUpdate = mInitialValue;
        }
        
        
    }
    else
    {
        if (mElapsedTimeMs > mDurationMs)
        {
            if (type() == Tween::Type::Loop || type() == Tween::Type::PingpongLoop)
            {
                mElapsedTimeMs = std::fmod(mElapsedTimeMs, mDurationMs);
            }
            
            if (type() == Tween::Type::PingpongLoop)
            {
                std::swap(mInitialValueInUpdate, mEndValueInUpdate);
            }

            else
            {
                mElapsedTimeMs = std::fmin(mElapsedTimeMs, mDurationMs);
            }

        }

        mValueSinceLastUpdate = mEasingFunc(mElapsedTimeMs, mInitialValueInUpdate, mEndValueInUpdate - mInitialValueInUpdate, mDurationMs);
    
    }
    
    if(hasFinished() && !mWasEndCallbackFuncCalled)
    {
        if (mEndCallbackFunc) mEndCallbackFunc(*this);
        
        mWasEndCallbackFuncCalled = true;
    }
}



