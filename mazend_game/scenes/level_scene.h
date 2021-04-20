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

	// Sector movement
	void UnLoadSector();
	void ChangeSector(Vector2i);
	virtual void DisplaySector();
	Vector2i getNewSector() const;
	void MovePlayerOnNewSector(Vector2i, Vector2i);

	// Player management
	void movePlayerTo(Vector2f);
	void setActivePlayer();
	void changeFloor(int);

protected:
	shared_ptr<Entity> _player1;
	shared_ptr<Entity> _player2;
	shared_ptr<Entity> _player3;
	shared_ptr<Entity> _activePlayer;

	float _playerCollisionVelue;

	Vector2i _activeSector;
	int _activePlayerFloor;

	TimeLimit _timeLimitValue;
	shared_ptr<Entity> _timeLimit;
};
