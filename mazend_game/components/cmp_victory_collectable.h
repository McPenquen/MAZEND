#pragma once
#include "engine.h"

class CollectableComponent : public Component {
private:
	vector<shared_ptr<Entity>> _players;
	int _activePlayerIndex;
	Vector2i _sectorId;

public:
	CollectableComponent(Entity* p, Vector2i sectorID, vector<shared_ptr<Entity>> player, int);
	CollectableComponent() = delete;
	~CollectableComponent() = default;

	void Update(double dt) override;
	void Render() override {}

	Vector2i getSector() const { return _sectorId; }
	void setSector(Vector2i newSector) { _sectorId = newSector; }

	void ChangeActivePlayerId(int playerIndex);
};