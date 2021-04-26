#pragma once
#include "../components/cmp_state_machine.h"

// The not-seeking state
class CasualState : public State {
private:
	shared_ptr<Entity> _player;

public:
	CasualState(shared_ptr<Entity> player) : _player(player) {}
	void Execute(Entity*, double) noexcept override;
};

// The seeking state
class HuntingState : public State {
private:
	shared_ptr<Entity> _player;

public:
	HuntingState(shared_ptr<Entity> player) : _player(player) {}
	void Execute(Entity*, double) noexcept override;
};