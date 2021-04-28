#include "cmp_victory_collectable.h"
#include "cmp_player_movement.h"
#include <LevelSystem.h>

int CollectableComponent::collectedAmount = 0;

CollectableComponent::CollectableComponent(Entity* p, Vector2i sectorID, vector<shared_ptr<Entity>> players, int activePlayerIndx) : Component(p) {
	_sectorId = sectorID;
	_players = players;
	_activePlayerIndex = activePlayerIndx;
}

void CollectableComponent::Update(double dt) {
	Vector2i playerSector = _players[_activePlayerIndex]->GetComponents<PlayerMovementComponent>()[0]->getSector();
	if (_sectorId == playerSector) {
		_parent->setVisible(true);
	}
	else {
		_parent->setVisible(false);
	}
	if (_parent->isVisible()) {
		if (length(_players[_activePlayerIndex]->getPosition() - _parent->getPosition()) <= _parent->getCollisionBounds()) {
			_parent->setAlive(false);
			collectedAmount++;
		}
	}
}

void CollectableComponent::ChangeActivePlayerId(int playerIndex) {
	_activePlayerIndex = playerIndex;
}