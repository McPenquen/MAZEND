#include "game_end_scenes.h"
#include "../game.h"
#include "../components/cmp_text.h"

// Game Over
void GameOverScene::Load() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f(Engine::GetWindowSize().x/2 - 100, Engine::GetWindowSize().y/2 - 100));
	auto t = txt->addComponent<TextComponent>(
		"GAME OVER\n\nPress ESC"
		);
	setSceneName("gameOverScene");
	setLoaded(true);
}

void GameOverScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}

// Victory
void VictoryScene::Load() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f(Engine::GetWindowSize().x / 2 - 100, Engine::GetWindowSize().y / 2 - 100));
	TimeLimit tl = LevelScene::getTimeLimit();
	auto t = txt->addComponent<TextComponent>(
		"VICTORY\n\nScore: " + to_string(int(tl.minutes)) + "," + to_string(int(tl.seconds)) + "\n\nPress ESC"
		);
	setSceneName("victoryScene");
	setLoaded(true);
}

void VictoryScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}