#pragma once
#include "engine.h"


class LevelScene : public Scene {
public:
	LevelScene() = default;
	~LevelScene() = default;

	void Load(string const s);
	void Update(const double dt) override;
	void UnLoad() override;

	void UnLoadSector();
	void ChangeSector(Vector2i);
	virtual void DisplaySector();

	Vector2i getNewSector();
	void MovePlayerOnNewSector(Vector2i, Vector2i);

protected:
	shared_ptr<Entity> _player;
	Vector2i _activeSector;
	
};
