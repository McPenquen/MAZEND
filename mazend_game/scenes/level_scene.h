#pragma once
#include "engine.h"

struct TimeLimit {
	float minutes;
	float seconds;
};

class LevelScene : public Scene {
public:
	LevelScene() = default;
	~LevelScene() = default;

	void Load(string const s);
	void Update(const double dt) override;
	void UnLoad() override;
	void Render() override;

	void UnLoadSector();
	void ChangeSector(Vector2i);
	virtual void DisplaySector();

	Vector2i getNewSector();
	void MovePlayerOnNewSector(Vector2i, Vector2i);

protected:
	shared_ptr<Entity> _player;
	Vector2i _activeSector;
	TimeLimit _timeLimitValue;
	shared_ptr<Entity> _timeLimit;
};
