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

	void Load(string const s, string const s1, string const s2);
	void Update(const double dt) override;
	void UnLoad() override;
	void Render() override;

	void UnLoadSector();
	void ChangeSector(Vector2i);
	virtual void DisplaySector();

	Vector2i getNewSector();
	void MovePlayerOnNewSector(Vector2i, Vector2i);

protected:
	shared_ptr<Entity> _player1;
	shared_ptr<Entity> _player2;
	shared_ptr<Entity> _player3;
	int _activePlayer;
	float _playerCollisionVelue;
	Vector2i _activeSector;
	TimeLimit _timeLimitValue;
	shared_ptr<Entity> _timeLimit;
};
