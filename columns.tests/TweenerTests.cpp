//
//  TweenerTests.cpp
//  Columns
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
        
        t.Restart();
        REQUIRE(t.hasStarted());
        REQUIRE(t.currentValue() == 0);
        
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
        
        t.Reset(5, 10, 30);
        
        
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
    
    SECTION("Can be paused")
    {
        Tween t(0, 10, 20, Linear::easeIn);
        
        t.Start();
        t.Update(10);
        
        REQUIRE(t.currentValue() == 5);
        
        t.Stop();
        
        REQUIRE(t.currentValue() == 5);
        REQUIRE_FALSE(t.isRunning());
        
        t.Update(5);
        
        REQUIRE(t.currentValue() == 5);
        t.Start();
        
        REQUIRE(t.isRunning());
        t.Update(5);
        
        REQUIRE(t.currentValue() == 7.5);
        
    }
}