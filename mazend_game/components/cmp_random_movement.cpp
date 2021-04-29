#include "cmp_random_movement.h"

RandomMovementComponent::RandomMovementComponent(Entity* p) : MovementComponent(p) {
	_collisionBounds = _parent->getCollisionBounds();
}

// Helper functions
float getRandomNumber(int maxVal, int minVal) {
	float answer = floor(rand() % maxVal + minVal);
	return answer;
}

void RandomMovementComponent::Update(double dt) {
	if (_parent->isAlive()) {
		// Random movement generation
		if (_directionChangeTime <= 0.0f) {
			float randSign1 = getRandomNumber(2, 0) == 1 ? -1.f : 1.f;
			float randSign2 = getRandomNumber(2, 0) == 1 ? -1.f : 1.f;
			float zeroChance1 = getRandomNumber(2, 0) == 1 ? 0.f : 1.f;
			float zeroChance2 = 1.f;
			if (zeroChance1 != 0.0f) {
				zeroChance2 = getRandomNumber(2, 0) == 1 ? 0.f : 1.f;
			}
			_movementDirection = { _speed * randSign1 * zeroChance1,
				_speed * randSign2 * zeroChance2 };
			_directionChangeTime = getRandomNumber(5, 1);
		}

		_directionChangeTime -= float(dt);
		// Colision detected movement
		if (_parent->getPosition().x - _collisionBounds > 0 &&
			_parent->getPosition().x + _collisionBounds < Engine::GetWindowSize().x &&
			_parent->getPosition().y - _collisionBounds > 0 &&
			_parent->getPosition().y + _collisionBounds < Engine::GetWindowSize().y) {
			move(Vector2f(_movementDirection.x * dt, _movementDirection.y * dt));
		}
		else {
			if (_parent->getPosition().x - _collisionBounds <= 0 && _movementDirection.x < 0) {
				_movementDirection.x *= -1.f;
			}
			else if (_parent->getPosition().x + _collisionBounds >= Engine::GetWindowSize().x && _movementDirection.x > 0) {
				_movementDirection.x *= -1.f;
			}
			if (_parent->getPosition().y - _collisionBounds <= 0 && _movementDirection.y < 0) {
				_movementDirection.y *= -1.f;
			}
			else if (_parent->getPosition().y + _collisionBounds >= Engine::GetWindowSize().y && _movementDirection.y > 0) {
				_movementDirection.y *= -1.f;
			}
			move(Vector2f(_movementDirection.x * dt, _movementDirection.y * dt));
		}

	}
}