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
	Vector2i playerSector = _players[_activePlayerIndex]->GetComponents<PlayerMovementComponent>()[0]->getSector();
	if (_sectorId == playerSector) {
		_parent->setVisible(true);
	}
	else {
		_parent->setVisible(false);
	}
	if (_parent->isAlive() && _isHunting) {
		Vector2f currentGlobalPos = LS::getGlobalPos(_parent->getPosition(), _sectorId);
		Vector2f globalDir = LS::getGlobalPos(_players[_activePlayerIndex]->getPosition(), playerSector) - currentGlobalPos;
		Vector2f newGlobalPos = currentGlobalPos + (globalDir / float(length(globalDir)) * _speed * float(dt));
		auto newLocalPos = LS::getLocalPos(newGlobalPos);
		_parent->setPosition(newLocalPos.position);
		_sectorId = newLocalPos.sectorId;
	}
}

void EnemyMovementComponent::changePlayerToHunt(int playerIndex) {
	_activePlayerIndex = playerIndex;
}