#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"

void Level1Scene::Load() {
	auto txt = makeEntity();
	auto t = txt->addComponent<TextComponent>(
		"LEVEL 1\n\nPause - Press 1"
		);
	setLoaded(true);
}

void Level1Scene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&pauseMenu);
	}
	Scene::Update(dt);
}