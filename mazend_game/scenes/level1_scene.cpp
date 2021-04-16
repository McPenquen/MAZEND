#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "LevelSystem.h"

void Level1Scene::Load() {
	LevelScene::Load("res/maps/lvl1/topMap.txt");

	// Set the time limit
	_timeLimitValue.minutes = 2.0f;
	_timeLimitValue.seconds = 0.0f;

	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"LEVEL 1\n\nPause - ESC"
		);
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
	txt->setPosition(Vector2f((gameWidth / 2) + 50, 100));
	string str = "                                                                           Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}
