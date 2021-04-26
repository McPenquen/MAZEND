#pragma once
#include "cmp_movement.h"

class EnemyMovementComponent : public MovementComponent {
private:
	bool _isHunting;
	vector<shared_ptr<Entity>> _players;
	int _activePlayerIndex;

public:
	EnemyMovementComponent(Entity* p, Vector2i sectorID, vector<shared_ptr<Entity>> player, int);
	EnemyMovementComponent() = delete;
	~EnemyMovementComponent() = default;
	void Update(double dt) override;

	void setHunting(bool b) { _isHunting = b; }
	void changePlayerToHunt(int playerIndex);
};