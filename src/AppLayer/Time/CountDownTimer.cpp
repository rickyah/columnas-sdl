//
//  CountDownTimer.cpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 22/6/16.
//
//

#include "CountDownTimer.hpp"


void CountDownTimer::Init(float countDownMs, float tickFrecuency, std::function<void(const CountDownTimer &)> tickFunction)
{
    mTickFrecuency = tickFrecuency;
    if (mTickFrecuency < 0) mTickFrecuency = 0;
    mTickFunc = tickFunction;
    
    mTweenCounter.Stop();
    mTweenCounter.initialValue(countDownMs).endValue(0).durationMs(countDownMs);
    
}

void CountDownTimer::Start()
{
    Stop();
    Restart();
}

void CountDownTimer::Pause()
{
    mTweenCounter.Pause();
}

void CountDownTimer::Resume()
{
    mTweenCounter.Resume();
}

void CountDownTimer::Stop()
{
    mTweenCounter.Stop();
}

void CountDownTimer::Restart()
{
    mTweenCounter.Stop();
    mElapsedForTicks = 0;
    
    mTweenCounter.Start();
}

void CountDownTimer::Update(float dt)
{
    if (!mTweenCounter.isRunning() || mTweenCounter.hasFinished() )
    {
        return;
    }
    
    mTweenCounter.Update(dt);
    
    if (mTickFrecuency <= 0 || !mTickFunc)
    {
        return;
    }
    
    mElapsedForTicks += dt;
    
    // call tick function
    while (mElapsedForTicks > mTickFrecuency)
    {
        mTickFunc(*this);
        
        mElapsedForTicks -= mTickFrecuency;
    }
    
    if (mTweenCounter.hasFinished())
    {
        mTickFunc(*this);
    }
}
