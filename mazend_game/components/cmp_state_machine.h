#pragma once
#include <engine.h>
#include <unordered_map>

using namespace std;
using namespace sf;

class State {
public:
	virtual ~State() = default;
	virtual void Execute(Entity*, double) noexcept = 0;
};

class StateMachineComponent : public Component {
private:
	unordered_map<string, shared_ptr<State>> _states;
	shared_ptr<State> _currentState;
	string _currentStateName = "";

public:
	void Update(double) override;
	void Render() override { }
	explicit StateMachineComponent(Entity* p);
	void addState(const string&, shared_ptr<State>) noexcept;
	shared_ptr<State> getState(const string&) const noexcept;
	void removeState(const string&) noexcept;
	void changeState(const string&) noexcept;
	const string& currentState() const { return _currentStateName; }
};