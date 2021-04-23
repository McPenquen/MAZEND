#pragma once
#include "cmp_movement.h"

class EnemyMovementComponent : public MovementComponent {
public:
	EnemyMovementComponent(Entity* p, Vector2i sectorID);
	EnemyMovementComponent() = delete;
	~EnemyMovementComponent() = default;
	void Update(double dt) override;
};