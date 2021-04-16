#include "game_end_scenes.h"
#include "../game.h"
#include "../components/cmp_text.h"

// Game Over
void GameOverScene::Load() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f(gameWidth/2 - 100, gameHeight/2 - 100));
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