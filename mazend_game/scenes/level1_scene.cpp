#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"

void Level1Scene::Load() {
	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"LEVEL 1\n\nPause - ESC"
		);
	setSceneName("level1");

	LevelScene::Load();
	
	setLoaded(true);
}

void Level1Scene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::PauseScene(&pauseMenu);
	}
	LevelScene::Update(dt);
}