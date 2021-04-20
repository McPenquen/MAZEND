#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "LevelSystem.h"

void Level1Scene::Load() {
	LevelScene::Load("res/maps/lvl1/botMap.txt", "res/maps/lvl1/midMap.txt", "res/maps/lvl1/topMap.txt");

	// Set the time limit
	_timeLimitValue.minutes = 2.0f;
	_timeLimitValue.seconds = 0.0f;

	// Load the initial sector and player position
	_activeSector = Vector2i(1, 2);
	_activePlayerFloor = 3;
	_playerCollisionVelue = tileBounds;
	DisplaySector();
	// Move the player to the starting position in the chosen sector corresponding to the correct floor
	movePlayerTo(Vector2f(leftXBorder + tileBounds * 3.0f, topYBorder + tileBounds * 3.0f));

	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"ESC for PAUSE"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("level1");

	setLoaded(true);
}

void Level1Scene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::PauseScene(&pauseMenu);
	}
	LevelScene::Update(dt);
}

void Level1Scene::DisplaySector() {
	//TODO: render the appropriate sector from the id
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f((gameWidth / 2) - 80,0));
	string str = "Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}
