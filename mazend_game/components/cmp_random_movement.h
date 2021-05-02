#pragma once
#include "cmp_movement.h"

class RandomMovementComponent : public MovementComponent {
private:
	float _directionChangeTime = 0.f;
	Vector2f _movementDirection;
	float _collisionBounds;
public:
	RandomMovementComponent(Entity*);
	RandomMovementComponent() = delete;
	~RandomMovementComponent() = default;
	void Update(double dt) override;
};