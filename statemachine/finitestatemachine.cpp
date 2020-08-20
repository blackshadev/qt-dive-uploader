#include "finitestatemachine.h"
#include <stdexcept>

template<typename TState>
FiniteStateMachine<TState>::FiniteStateMachine(TState initial)
{
    initialState = initial;
    currentState = getInitialState();
}

template<typename TState>
TState FiniteStateMachine<TState>::getInitialState()
{
    return initialState;
}

template<typename TState>
TState FiniteStateMachine<TState>::getCurrentState()
{
    return currentState;
}

template<typename TState>
bool FiniteStateMachine<TState>::canTransition(TState to)
{
    return getTransition(currentState, to) != NULL;
}

template<typename TState>
bool FiniteStateMachine<TState>::advance()
{
    if (transitioning) {
        return false;
    }

    transitioning = true;
    StateData<TState> &transition = getFirstTransition(getCurrentState());
    if (transition == NULL) {
        return false;
    }

    transition.action([=]() {
        transitioning = false;
    });

    return true;
}

template<typename TState>
void FiniteStateMachine<TState>::addTransition(TState from, TState to)
{
    return addTransition(from, to, NULL, NULL);
}

template<typename TState>
void FiniteStateMachine<TState>::addTransition(TState from, TState to, std::function<bool ()> condition, std::function<void (std::function<void()>)> action)
{

    if (stateMap.count(from) == 0) {
        std::map<TState, StateData<TState>> newMap;
        stateMap[from] = newMap;
    }

    std::map<TState, StateData<TState>> &currentStateMap = stateMap[from];

    if (currentStateMap.count(to) != 0) {
        throw std::invalid_argument("Given transition already present in state machine");
    }

    currentStateMap[to] =  {
        action,
        condition
    };
}


template<typename TState>
StateData<TState> *FiniteStateMachine<TState>::getFirstTransition(TState from)
{
    if (stateMap.count(from) == 0) {
        return NULL;
    }

    std::vector<StateData<TState>> &possibleStates = stateMap[from];

    for(StateData<TState> state : possibleStates) {
        if (state.condition()) {
            return &state;
        }
    }

    return NULL;
}
