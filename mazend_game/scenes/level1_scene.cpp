#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "LevelSystem.h"
#include "../components/cmp_player_movement.h"

void Level1Scene::Load() {
	// Load the initial sector and player position
	_activeSector = Vector2i(1, 2);
	_activePlayerFloor = 3;
	_playerCollisionVelue = tileBounds;
	DisplaySector();

	LevelScene::Load("res/maps/lvl1/botMap.txt", "res/maps/lvl1/midMap.txt", "res/maps/lvl1/topMap.txt");

	// Set the time limit
	_timeLimitValue.minutes = 2.0f;
	_timeLimitValue.seconds = 0.0f;

	// Move the player to the starting position in the chosen sector corresponding to the correct floor
	movePlayerTo(Vector2f(leftXBorder + tileBounds * 3.0f, topYBorder + tileBounds * 3.0f));
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

static float tempCounter = 5.0f;

void Level1Scene::Update(const double dt) {
	tempCounter -= dt;

	LevelScene::Update(dt);
	if (tempCounter <= 0.0) {
		tempCounter = 5.0f;
		auto answr = LS::getTileAt(_player1->getPosition(), _activeSector, _activePlayerFloor);
		string str = "not_found";
		if (answr == LS::EMPTY) { str = "empty"; }
		
		cout << str << endl;
	}
}

void Level1Scene::DisplaySector() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f((gameWidth / 2) - 80,0));
	string str = "Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}
