#include "cmp_enemy_movement.h"
#include "../game.h"
#include "LevelSystem.h"
#include "cmp_player_movement.h"

EnemyMovementComponent::EnemyMovementComponent(Entity* p, Vector2i sectorID, vector<shared_ptr<Entity>> players, int activePlayerIndx) : MovementComponent(p) {
	_sectorId = sectorID; 
	_isHunting = false; 
	_players = players;
	_activePlayerIndex = activePlayerIndx;
}

void EnemyMovementComponent::Update(double dt) {
	if (_sectorId ==
		_players[_activePlayerIndex]->GetComponents<PlayerMovementComponent>()[0]->getSector()) {
		_parent->setVisible(true);
	}
	else {
		_parent->setVisible(false);
	}
	if (_parent->isAlive() && _isHunting && _parent->isVisible()) {
		Vector2f dir = _players[_activePlayerIndex]->getPosition() - _parent->getPosition();
		move(dir / float(length(dir)) * _speed * float(dt));
	}
}

void EnemyMovementComponent::changePlayerToHunt(int playerIndex) {
	_activePlayerIndex = playerIndex;
}