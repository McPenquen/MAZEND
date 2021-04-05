#pragma once
#include "cmp_movement.h"

class PlayerMovementComponent : public MovementComponent {
public:
	PlayerMovementComponent(Entity* p);
	PlayerMovementComponent() = delete;
	~PlayerMovementComponent() = default;
	void Update(double dt) override;
};