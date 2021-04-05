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

		if (_parent->getPosition().x - tileBounds > 0 &&
			_parent->getPosition().x + tileBounds < gameWidth &&
			_parent->getPosition().y - tileBounds > 0 &&
			_parent->getPosition().y + tileBounds < gameHeight) {
			move(direction);
		}
		else {
			if (_parent->getPosition().x - 15.f <= 0 && direction.x < 0) {
				direction.x = 0;
			}
			else if (_parent->getPosition().x + 15.f >= gameWidth && direction.x > 0) {
				direction.x = 0;
			}
			if (_parent->getPosition().y - 15.f <= 0 && direction.y < 0) {
				direction.y = 0;
			}
			else if (_parent->getPosition().y + 15.f >= gameHeight && direction.y > 0) {
				direction.y = 0;
			}
			move(direction);
		}
	}
}