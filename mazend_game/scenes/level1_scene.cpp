#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_shape.h"
#include "lib_tile_level_system/LevelSystem.h"
#include "../components/cmp_player_movement.h"

void Level1Scene::Load() {
	auto txt = makeEntity();
	auto t = txt->addComponent<TextComponent>(
		"LEVEL 1\n\nPause - ESC"
		);
	setSceneName("level1");

	LevelScene::Load();
	
	// Create the player
	auto pl = makeEntity();
	pl->setNameTag("player");
	auto plS = pl->addComponent<ShapeComponent>();
	float plRad = tileBounds;
	plS->setShape<CircleShape>(plRad);
	const Color plColor = {100, 250, 61};
	plS->getShape().setFillColor(plColor);
	plS->getShape().setOutlineColor(Color::Black);
	plS->getShape().setOutlineThickness(2.f);
	plS->getShape().setOrigin(Vector2f(plRad, plRad));
	pl->setPosition(Vector2f(gameWidth/2, gameHeight/2));
	auto plM = pl->addComponent<PlayerMovementComponent>();
	plM->setSpeed(500.f);
	player = pl;
	
	setLoaded(true);
}

void Level1Scene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene(&pauseMenu);
	}
	Scene::Update(dt);
}