#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H
#include "statemachineinterface.h"
#include <map>
#include <vector>

enum BaseState {};

template<typename TState>
class FiniteStateMachine : public StateMachineInterface<TState>
{
public:
    FiniteStateMachine(TState initial);
    virtual TState getInitialState() override;
    virtual TState getCurrentState() override;
    virtual bool canTransition(TState to) override;
    virtual bool advance() override;
    virtual void addTransition(TState from, TState to) override;
    virtual void addTransition(TState from, TState to, std::function<bool ()> condition, std::function<void (std::function<void()>)> action) override;
    virtual bool isTransitioning() override;
private:
    std::map<TState, std::vector<StateData<TState>>> stateMap;
    TState initialState;
    TState currentState;
    bool transitioning = false;
    StateData<TState> *getFirstTransition(TState from);
};

template<>
class FiniteStateMachine<BaseState>;

#endif // FINITESTATEMACHINE_H
