#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"
#include "../components/cmp_player_movement.h"

void LevelScene::Load() {
	// Identify active sector

	// Create the mid sector
	auto sector = makeEntity(1);
	auto ss = sector->addComponent<ShapeComponent>();
	ss->setShape<RectangleShape>(sectorBounds);
	ss->getShape().setFillColor(Color::Black);
	ss->getShape().setOrigin(sectorBounds / 2.f);
	ss->getShape().setOutlineColor(Color::White);
	ss->getShape().setOutlineThickness(5.f);
	sector->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));

	// Create the player
	auto pl = makeEntity(3);
	pl->setNameTag("player");
	auto plS = pl->addComponent<ShapeComponent>();
	float plRad = tileBounds;
	plS->setShape<CircleShape>(plRad);
	const Color plColor = { 222, 120, 31 }; // #DE781F
	plS->getShape().setFillColor(plColor);
	plS->getShape().setOutlineColor(Color::Black);
	plS->getShape().setOutlineThickness(2.f);
	plS->getShape().setOrigin(Vector2f(plRad, plRad));
	pl->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));
	auto plM = pl->addComponent<PlayerMovementComponent>();
	plM->setSpeed(500.f);
	_player = pl;
}

void LevelScene::Render() {
	// TODO change the rendering order so the player can be rendered in between layers
	_activeSector->Render();
	Scene::Render();
}

// Sector
void Sector::Render() {

}
