#include "cmp_victory_collectable.h"
#include "cmp_player_movement.h"
#include <LevelSystem.h>

int CollectableComponent::collectedAmount = 0;
//audio is from https://freesound.org/people/ProjectsU012/sounds/341695/ by ProjectsU012
sf::Sound collect;
sf::SoundBuffer buffer;
CollectableComponent::CollectableComponent(Entity* p, Vector2i sectorID, vector<shared_ptr<Entity>> players, int activePlayerIndx) : Component(p) {
	_sectorId = sectorID;
	_players = players;
	_activePlayerIndex = activePlayerIndx;
	buffer.loadFromFile("res/audio/collection.wav");
	collect.setBuffer(buffer);
	collect.setVolume(5);
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
			collect.play();
			collectedAmount++;
		}
	}
}

void CollectableComponent::ChangeActivePlayerId(int playerIndex) {
	_activePlayerIndex = playerIndex;
}