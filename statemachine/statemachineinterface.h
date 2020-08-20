#ifndef STATEMACHINEINTERFACE_H
#define STATEMACHINEINTERFACE_H
#include <functional>

template <typename TState>
struct StateData {
    std::function<void ( std::function<void ()>)> action;
    std::function<bool ()> condition;
};

template <typename TState>
class StateMachineInterface
{
public:
    virtual TState getInitialState() = 0;
    virtual TState getCurrentState() = 0;
    virtual bool canTransition(TState to) = 0;
    virtual void addTransition(TState from, TState to) = 0;
    virtual void addTransition(TState from, TState to, std::function<bool ()> condition, std::function<void (std::function<void()>)> action) = 0;
    virtual bool isTransitioning() = 0;
};

#endif // STATEMACHINEINTERFACE_H
