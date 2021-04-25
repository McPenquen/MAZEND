#include "menu_scenes.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_shape.h"

// Main Menu
void MainMenuScene::Load() {
	auto txt = makeEntity(1);
	auto t = txt->addComponent<TextComponent>(
		"MAIN MENU\n\nPlay Levels - Press 1\nOptions - Press 2\nExit - Press 3"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
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
		"OPTIONS\n\nWindow Mode:\n   Window Mode (Press W+1) VS Full Screen (Press W+2)\n\nBack - Press 9"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("options");
	setLoaded(true);
}

void OptionsScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeWindowMode("default");
	}
	if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeWindowMode("fullscreen");
	}
	if (Keyboard::isKeyPressed(Keyboard::Num9)) {
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
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
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
	auto curtain = makeEntity(4);
	auto cs = curtain->addComponent<ShapeComponent>();
	cs->setShape<RectangleShape>(Vector2f(Engine::GetWindowSize().x, Engine::GetWindowSize().y));
	cs->getShape().setFillColor(Color::Black);
	cs->getShape().setOrigin(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	curtain->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"PAUSE\n\nResume - Press 1\nLeave the level - Press 2"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("pauseMenu");
	setLoaded(true);
}

void PauseMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::UnloadPreviousScene();
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}
