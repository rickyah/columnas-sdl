//
//  TweenerTests.cpp
//
//
//  Created by Ricardo Amores Hernández on 12/6/16.
//
//

#include "catch.hpp"
#include "Tween.hpp"

TEST_CASE("Test Tween API", "[App]") {
    
    SECTION("Initialization setups correct values") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        REQUIRE_FALSE(t.hasStarted());
        REQUIRE_FALSE(t.hasFinished());
        REQUIRE_FALSE(t.isRunning());
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 0);
        REQUIRE(t.durationMs() == 20.0);
    }
    
    
    SECTION("Tween needs to be started to work") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Update(100);
        
        REQUIRE_FALSE(t.isRunning());
        REQUIRE_FALSE(t.hasStarted());
        REQUIRE_FALSE(t.hasFinished());
        
        REQUIRE(t.currentValue() == 0);
    }
    
    
    SECTION("Updating the Tween updates the inner values") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Start();
        
        REQUIRE(t.isRunning());
        t.Update(10);
        
        
        REQUIRE(t.isRunning());
        REQUIRE_FALSE(t.hasFinished());
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 5);
        REQUIRE(t.durationMs() == 20.0);
    }
    
    SECTION("Check Tween finishes") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Start();
        t.Update(100);
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 10);
        REQUIRE(t.durationMs() == 20.0);
        
        REQUIRE(t.hasFinished());
        REQUIRE(t.hasStarted());
        REQUIRE_FALSE(t.isRunning());
    }
    
    SECTION("Check Tween can be restarted") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Start();
        t.Update(100);
        
        REQUIRE(t.hasFinished());
        REQUIRE(t.hasStarted());
        REQUIRE(t.currentValue() == 10);
        
        // Starting an already started tween has no effect
        t.Start();
        REQUIRE(t.hasStarted());
        REQUIRE(t.hasFinished());
        REQUIRE(t.currentValue() == 10);
        
        
        // now we are talking ;)
        t.Stop();
        t.Start();
        t.Update(10);
        REQUIRE(t.currentValue() == 5);
        
        REQUIRE(t.hasStarted());
        REQUIRE(t.isRunning());
        REQUIRE_FALSE(t.hasFinished());
    }
    
    SECTION("Check Tween can be restarted with different values") {
        
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Start();
        t.Update(10);
        
        REQUIRE_FALSE(t.hasFinished());
        
        t.Stop();
        t.initialValue(5).endValue(10).durationMs(30);
        
        REQUIRE(t.initialValue() == 5);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.durationMs() == 30);
        REQUIRE(t.currentValue() == 5);
        
        REQUIRE_FALSE(t.hasStarted());
        REQUIRE_FALSE(t.hasFinished());
        
        t.Start();
        t.Update(15);
        
        REQUIRE(t.initialValue() == 5);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.durationMs() == 30);
        REQUIRE(t.currentValue() == 7.5);
        
        REQUIRE(t.isRunning());
        REQUIRE_FALSE(t.hasFinished());
        
        t.Update(15);
        REQUIRE(t.hasFinished());
    }
    
    SECTION("Can be paused") {
        Tween t(0, 10, 20, Linear::easeIn);
        
        t.Start();
        t.Update(10);
        
        REQUIRE(t.currentValue() == 5);
        
        t.Pause();
        
        REQUIRE(t.currentValue() == 5);
        REQUIRE_FALSE(t.isRunning());
        
        t.Update(5);
        
        REQUIRE(t.currentValue() == 5);
        t.Resume();
        
        REQUIRE(t.isRunning());
        t.Update(5);
        
        REQUIRE(t.currentValue() == 7.5);
        
    }
    
    SECTION("Check we can add an initial delay for the animation") {
        Tween t(0, 10, 20, Linear::easeIn);
        
        t.initialDelayMs(10);
        
        REQUIRE(t.durationMs() == 20);
        
        t.Start();
        t.Update(5);
        REQUIRE(t.currentValue() == 0);
        
        t.Update(2);
        REQUIRE(t.currentValue() == 0);
        
        // Here we passed the initial delay for 2ms, so the current value should update
        t.Update(5);
        REQUIRE(t.currentValue() == 1);
        
        t.Update(10);
        
        REQUIRE(t.currentValue() == 6);
    }
    
    SECTION("Check reverse animation") {
        Tween t(0, 10, 20, Linear::easeIn);
        
        t.type(Tween::Type::Reverse);
        
        t.Start();
        REQUIRE(t.currentValue() == 10);
        
        t.Update(2);
        REQUIRE(t.currentValue() == 9);
        
        t.Update(8);
        REQUIRE(t.currentValue() == 5);
        
        t.Update(100);
        
        REQUIRE(t.currentValue() == 0);
        REQUIRE(t.hasFinished());
    }
    
    SECTION("Check loop animation") {
        Tween t(0, 10, 20, Linear::easeIn);
        t.type(Tween::Type::Loop).Start();
        
        REQUIRE(t.currentValue() == 0);
        
        t.Update(30);
        
        REQUIRE_FALSE(t.hasFinished());
        REQUIRE(t.currentValue() == 5);
        
        t.Update(1000);
        REQUIRE_FALSE(t.hasFinished());
    }
    
    SECTION("Check ping-pong animation") {
        Tween t(0, 10, 20, Linear::easeIn);
        t.type(Tween::Type::Pingpong).Start();
        
        REQUIRE(t.currentValue() == 0);
        
        t.Update(15);
        
        REQUIRE_FALSE(t.hasFinished());
        REQUIRE(t.currentValue() == 7.5);
        
        t.Update(20);
        REQUIRE_FALSE(t.hasFinished());
        REQUIRE(t.currentValue() == 2.5);
        
        t.Update(10);
        REQUIRE(t.hasFinished());
    }
    
    SECTION("Check ping-pong loooping animation") {
        Tween t(0, 10, 20, Linear::easeIn);
        t.type(Tween::Type::PingpongLoop).Start();
        
        REQUIRE(t.currentValue() == 0);
        
        t.Update(22);
        
        REQUIRE_FALSE(t.hasFinished());
        REQUIRE(t.currentValue() == 9);
        
        t.Update(20);
        REQUIRE_FALSE(t.hasFinished());
        REQUIRE(t.currentValue() == 1);
        
        t.Update(1000);
        REQUIRE_FALSE(t.hasFinished());
    }
    
    SECTION("Check end callback function is called") {
        Tween t(0, 10, 20, Linear::easeIn);
        t.type(Tween::Type::Pingpong);
        bool hasFinishedVar = false;
        
        t.endCallback([&hasFinishedVar](const Tween & tween) {
            hasFinishedVar = true;
        });
        
        t.Start();
        t.Update(40);
        
        REQUIRE(t.hasFinished());
        REQUIRE(hasFinishedVar == true);
    }
    
    
}
