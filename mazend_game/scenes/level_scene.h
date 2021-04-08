#pragma once
#include "engine.h"


class LevelScene : public Scene {
public:
	LevelScene() = default;
	~LevelScene() = default;

	void Load() override;
	void Update(const double dt) override;

	void UnLoadSector();
	void ChangeSector(Vector2f);

protected:
	shared_ptr<Entity> _player;
	Vector2f _activeSector;
	vector<Vector2f> _sectors;
};
