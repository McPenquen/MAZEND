#pragma once
#include "engine.h"

// Sector
struct Sector {
	Vector2f id;
	void Render();
};

class LevelScene : public Scene {
public:
	LevelScene() = default;
	~LevelScene() = default;

	void Load() override;
	void Render() override;

protected:
	shared_ptr<Entity> _player;
	shared_ptr<Sector> _activeSector;
	map<Vector2f, shared_ptr<Sector>> _sectors;
};
