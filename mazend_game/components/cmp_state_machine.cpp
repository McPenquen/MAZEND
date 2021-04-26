#include "cmp_state_machine.h"

using namespace std;

StateMachineComponent::StateMachineComponent(Entity* p) : _currentState(nullptr), Component(p) { }

void StateMachineComponent::Update(double dt)
{
    if (_currentState != nullptr) {
        _currentState->Execute(_parent, dt);
    }
}

void StateMachineComponent::addState(const string& name, shared_ptr<State> state) noexcept {
    _states[name] = state;
}

shared_ptr<State> StateMachineComponent::getState(const string& name) const noexcept {
    auto found = _states.find(name);
    if (found != _states.end()) {
        return found->second;
    }
    else {
        return nullptr;
    }
}

void StateMachineComponent::removeState(const string& name) noexcept {
    _states.erase(name);
}

void StateMachineComponent::changeState(const string& name) noexcept {
    auto found = _states.find(name);
    if (found != _states.end()) {
        _currentState = found->second;
        _currentStateName = name;
    }
}