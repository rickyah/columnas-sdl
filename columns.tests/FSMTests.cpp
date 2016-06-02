//
//  FSMTests.cpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 1/6/16.
//
//

#include "catch.hpp"
#include "FSM.hpp"

TEST_CASE( "FSM", "[App Layer]" ) {
    
    enum StateIds { MyState = 0, MyState2 = 1};
    
    class MyState : public IState
    {
    public:
        enum EMethodCalled
        {
            FlagNone = 0,
            FlagOnInit = 1,
            FlagOnCleanup = 2,
            FlagOnEnter = 4,
            FlagOnExit = 8,
        };
        
        virtual void OnInit()
        {
            value = value | EMethodCalled::FlagOnInit;
        }
        virtual void OnCleanup()
        {
            value = value | EMethodCalled::FlagOnCleanup;
        }
        virtual void OnEnter()
        {
            value = value | EMethodCalled::FlagOnEnter;
        }
        virtual void OnExit()
        {
            value = value | EMethodCalled::FlagOnExit;
        }
        
        MyState():IState(),value(EMethodCalled::FlagNone){}
        
        int value;
    
    };
    
    class MyState2 : public MyState
    {
    public:
        
        MyState2():MyState() {}
    };
    
    SECTION("FSM does not crash if no state is set") {
        
        FSM<int, IState> m;
        
        m.Update(0);
    }
    
    SECTION("FSM is initialized correctly") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        REQUIRE(m.currentState() == nullptr);
    }
    
    SECTION("Adding an state inits the method calling the OnInit method") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        REQUIRE(first->value == MyState::FlagOnInit);
        REQUIRE(second->value == MyState::FlagOnInit);
    }
    
    SECTION("Removing an state cleans it up calling the OnCleanup method") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        m.RemoveState(StateIds::MyState2);
        
        REQUIRE((second->value & MyState::FlagOnExit) == 0);
        REQUIRE((second->value & MyState::FlagOnCleanup) == MyState::FlagOnCleanup);
    }
    
    SECTION("Removing the *current state* cleans up it up calling the OnExit and OnCleanup methods") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        
        m.RemoveState(StateIds::MyState);
        
        REQUIRE((first->value & MyState::FlagOnExit) == MyState::FlagOnExit);
        REQUIRE((first->value & MyState::FlagOnCleanup) == MyState::FlagOnCleanup);
        
        REQUIRE(m.currentState() == nullptr);
    }

    SECTION("Changing to a state calls OnEnter") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        
        m.RegisterState(StateIds::MyState, first);
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        
        REQUIRE(m.currentState() == first);
        
        REQUIRE((first->value & MyState::FlagOnEnter) == MyState::FlagOnEnter);
    }
    
    
    SECTION("Changing to a new state calls OnExit on the previous one, and OnEnter in the new one") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        m.ChangeTo(StateIds::MyState2);
        m.Update(0);
        
        REQUIRE(m.currentState() == second);
        REQUIRE((first->value & MyState::FlagOnEnter) == MyState::FlagOnEnter);
        REQUIRE((first->value & MyState::FlagOnExit) == MyState::FlagOnExit);
        REQUIRE((second->value & MyState::FlagOnEnter) == MyState::FlagOnEnter);
    }
    
    SECTION("Leaving an state calls OnExit") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        
        m.RegisterState(StateIds::MyState, first);
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        
        REQUIRE(m.currentState() == first);
        REQUIRE((first->value & MyState::FlagOnEnter) == MyState::FlagOnEnter);
    }
    
    SECTION("FSM changes states only after calling update") {
        
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        REQUIRE(m.currentState() == nullptr);
        
        m.ChangeTo(StateIds::MyState);
        
        REQUIRE(m.currentState() == nullptr);
        
        m.Update(0);
        
        REQUIRE(m.currentState() == first);
        
        m.ChangeTo(StateIds::MyState2);

        REQUIRE(m.currentState() == first);
        
        m.Update(0);
        
        REQUIRE(m.currentState() == second);
    }
    
    SECTION("Changing to the same current state does not call OnExit and OnEnter") {
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        
        //
        first->value = MyState::FlagNone;
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        
        REQUIRE(first->value == MyState::FlagNone);
    }
    
    SECTION("Changing to an unregistered state returns false") {
        FSM<int, IState> m;
        
        REQUIRE_FALSE(m.ChangeTo(4));
    }
    
    SECTION("You can't register two states with the same Id") {
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        m.RegisterState(StateIds::MyState, first);
        
        REQUIRE_FALSE(m.RegisterState(StateIds::MyState, second));
    }
    
    SECTION("You can't remove an state that was not registered") {
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        m.RegisterState(StateIds::MyState, first);
        
        REQUIRE_FALSE(m.RemoveState(StateIds::MyState2));
    }
    
    SECTION("Changing to an state and removing it before Updating does not performs the state change") {
        FSM<int, IState> m;
        
        auto first = std::make_shared<class MyState>();
        auto second = std::make_shared<class MyState2>();
        m.RegisterState(StateIds::MyState, first);
        m.RegisterState(StateIds::MyState2, second);
        
        m.ChangeTo(StateIds::MyState);
        m.Update(0);
        
        m.ChangeTo(StateIds::MyState2);
        m.RemoveState(StateIds::MyState2);
        m.Update(0);
        
        REQUIRE(m.currentState() == first);
    }
}