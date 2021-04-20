#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "LevelSystem.h"
#include "../components/cmp_player_movement.h"

void Level1Scene::Load() {
	// Load the initial sector and player position
	_activeSector = Vector2i(2, 2);
	_activePlayerFloor = 2;
	_playerCollisionVelue = tileBounds;
	DisplaySector();

	LevelScene::Load("res/maps/lvl1/botMap.txt", "res/maps/lvl1/midMap.txt", "res/maps/lvl1/topMap.txt");

	// Set the time limit
	_timeLimitValue.minutes = 2.0f;
	_timeLimitValue.seconds = 0.0f;

	// Move the player to the starting position in the chosen sector corresponding to the correct floor
	movePlayerTo(Vector2f(leftXBorder + tileBounds * 9.0f, topYBorder + tileBounds * 7.0f));
	// Set the active player
	setActivePlayer();

	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"ESC for PAUSE"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("level1");

	setLoaded(true);
}

void Level1Scene::Update(const double dt) {
	LevelScene::Update(dt);
}

void Level1Scene::DisplaySector() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f((gameWidth / 2) - 80,0));
	string str = "Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}
