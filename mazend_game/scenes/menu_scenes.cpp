#include "menu_scenes.h"
#include "../components/cmp_text.h"
#include "../game.h"

// Main Menu
void MainMenuScene::Load() {
	auto txt = makeEntity(1);
	auto t = txt->addComponent<TextComponent>(
		"MAIN MENU\n\nPlay Levels - Press 1\nOptions - Press 2\nExit - Press 3"
		);
	setSceneName("mainMenu");
	setLoaded(true);
}

void MainMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&levels);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&options);
	}
	Scene::Update(dt);
}

 // Options
void OptionsScene::Load() {
	auto txt = makeEntity(1);
	auto t = txt->addComponent<TextComponent>(
		"OPTIONS\n\nBack - Press 1"
		);
	setSceneName("options");
	setLoaded(true);
}

void OptionsScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&mainMenu);
	}
	Scene::Update(dt);
}

// Levels
void LevelsScene::Load() {
	auto txt = makeEntity(1);
	auto t = txt->addComponent<TextComponent>(
		"LEVELS\n\nLevel 1 - Press 1\nBack - Press 2"
		);
	setSceneName("levels");
	setLoaded(true);
}

void LevelsScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&mainMenu);
	}
	Scene::Update(dt);
}

// Pause Menu
void PauseMenuScene::Load() {
	auto txt = makeEntity(1);
	auto t = txt->addComponent<TextComponent>(
		"PAUSE\n\nResume - Press 1\nLeave the level - Press 2"
		);
	setSceneName("pauseMenu");
	setLoaded(true);
}

void PauseMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}
