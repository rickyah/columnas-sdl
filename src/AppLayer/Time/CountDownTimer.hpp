//
//  CountDownTimer.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 22/6/16.
//
//

#ifndef CountDownTimer_hpp
#define CountDownTimer_hpp

#include <functional>
#include <cmath>

#include "Tween.hpp"

class CountDownTimer
{
public:
    
    void Init(float countDownMs, float tickFrecuency = 0, std::function<void(const CountDownTimer &)> tickFunc=nullptr);
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void Restart();
    void Update(float dt);
    
    bool hasFinished() const { return mTweenCounter.hasFinished();}
    float remainingMs() const { return mTweenCounter.currentValue(); }
    int remainingSecs() const { return static_cast<int>(std::round(remainingMs()/1000)); }
    float countDownMs() const { return mTweenCounter.initialValue(); }
    int countDownSecs() const { return static_cast<int>(std::round(countDownMs()/1000)); }
    
private:
    
    float mElapsedForTicks;
    float mTickFrecuency = 0;
    
    // Use a tween with linear interpolation as a countdown to save time
    Tween mTweenCounter;
    std::function<void(const CountDownTimer &)> mTickFunc;
};
#endif /* CountDownTimer_hpp */
