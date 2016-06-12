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
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 0);
        REQUIRE(t.durationMs() == 20.0);
        REQUIRE_FALSE(t.isFinished());
    }
    
    SECTION("Updating the Tween updates the inner values") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Update(10);
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 5);
        REQUIRE(t.durationMs() == 20.0);
        
        REQUIRE_FALSE(t.isFinished());
    }
    
    SECTION("Check Tween finishes") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Update(100);
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 10);
        REQUIRE(t.durationMs() == 20.0);
        
        REQUIRE(t.isFinished());
    }
    
    SECTION("Check Tween can be restarted") {
        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Update(100);
        
        REQUIRE(t.isFinished());
        
        t.Restart();
        
        REQUIRE(t.initialValue() == 0);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.currentValue() == 0);
        REQUIRE(t.durationMs() == 20.0);
        
        REQUIRE_FALSE(t.isFinished());
    }

    SECTION("Check Tween can be restarted with different values") {

        Tween t(0, 10.0, 20.0, Linear::easeIn);
        
        t.Update(100);
        
        REQUIRE(t.isFinished());
        
        t.Restart(5, 10, 30);
        
        REQUIRE(t.initialValue() == 5);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.durationMs() == 30);
        REQUIRE(t.currentValue() == 5);
        
        REQUIRE_FALSE(t.isFinished());
        
        t.Update(15);
        
        REQUIRE(t.initialValue() == 5);
        REQUIRE(t.endValue() == 10);
        REQUIRE(t.durationMs() == 30);
        REQUIRE(t.currentValue() == 7.5);
        
        REQUIRE_FALSE(t.isFinished());
        
        t.Update(15);
        REQUIRE(t.isFinished());
    }
}