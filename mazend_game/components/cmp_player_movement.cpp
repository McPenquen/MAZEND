#include "cmp_player_movement.h"
#include "../game.h"
#include "LevelSystem.h"

PlayerMovementComponent::PlayerMovementComponent(Entity* p) : MovementComponent(p) {}

void PlayerMovementComponent::Update(double dt) {
	if (_parent->isAlive()) {
		Vector2f direction = { 0.f, 0.f };
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			direction.x = -_speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			direction.x = _speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			direction.y = _speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			direction.y = -_speed * dt;
		}

		float sectorXLoc = gameWidth / 2 - sectorBounds.x / 2;
		float sectorYLoc = gameHeight / 2 - sectorBounds.y / 2;

		if (_parent->getPosition().x - tileBounds > sectorXLoc &&
			_parent->getPosition().x + tileBounds < (sectorXLoc + sectorBounds.x) &&
			_parent->getPosition().y - tileBounds > sectorYLoc &&
			_parent->getPosition().y + tileBounds < (sectorYLoc + sectorBounds.y)) {
			move(direction);
		}
		else {
			if (_parent->getPosition().x - tileBounds <= sectorXLoc && direction.x < 0) {
				direction.x = 0;
			}
			else if (_parent->getPosition().x + tileBounds >= (sectorXLoc + sectorBounds.x) && direction.x > 0) {
				direction.x = 0;
			}
			if (_parent->getPosition().y - tileBounds <= sectorYLoc && direction.y < 0) {
				direction.y = 0;
			}
			else if (_parent->getPosition().y + tileBounds >= (sectorYLoc + sectorBounds.y) && direction.y > 0) {
				direction.y = 0;
			}
			move(direction);
		}
	}
}