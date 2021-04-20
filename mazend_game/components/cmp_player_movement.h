#pragma once
#include "cmp_movement.h"

class PlayerMovementComponent : public MovementComponent {
public:
	PlayerMovementComponent(Entity* p, Vector2i sectorID);
	PlayerMovementComponent() = delete;
	~PlayerMovementComponent() = default;
	void Update(double dt) override;

	bool validMove(const Vector2f& pos) override;
};