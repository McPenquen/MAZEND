#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"
#include "../components/cmp_player_movement.h"

void CheckSectorChange() {

}

void LevelScene::Load() {
	//Load the initial sector
	//TODO: get position of the player to start at
	ChangeSector(Vector2f(1,1));

	// Create the mid sector
	auto sector = makeEntity(4);
	auto ss = sector->addComponent<ShapeComponent>();
	ss->setShape<RectangleShape>(sectorBounds);
	ss->getShape().setFillColor(Color::Black);
	ss->getShape().setOrigin(sectorBounds / 2.f);
	ss->getShape().setOutlineColor(Color::White);
	ss->getShape().setOutlineThickness(5.f);
	sector->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));

	// Create the player
	auto pl = makeEntity(4);
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

void LevelScene::Update(double const dt) {
	Scene::Update(dt);
	CheckSectorChange();
}

void LevelScene::ChangeSector(Vector2f sectorId) {
	_activeSector = sectorId;
	UnLoadSector();
	//TODO: render the appropriate sector from the id
}

void LevelScene::UnLoadSector() {
	ents.floor1_list.clear();
	ents.floor2_list.clear();
	ents.floor3_list.clear();
}