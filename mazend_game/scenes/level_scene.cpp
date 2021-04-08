#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"
#include "../components/cmp_player_movement.h"
#include "../components/cmp_text.h"

void LevelScene::Load() {
	//Load the initial sector
	//TODO: get position of the player to start at
	_activeSector = Vector2i(1, 1);
	DisplaySector();

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
	Vector2i nv = getNewSector();
	if (nv != Vector2i(0, 0)) {
		ChangeSector(nv);
	}
}

void LevelScene::DisplaySector() {
	//TODO: render the appropriate sector from the id
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f((gameWidth / 2) + 50, 100));
	string str = "                                                      Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}

void LevelScene::ChangeSector(Vector2i sectorId) {
	// Reset the path tiles and collectables
	UnLoadSector();
	DisplaySector();
	// Move player to the other side of the square
	MovePlayerOnNewSector(_activeSector, sectorId);
	_activeSector = sectorId;
}

void LevelScene::UnLoadSector() {
	ents.floor1_list.clear();
	ents.floor2_list.clear();
	ents.floor3_list.clear();
}

// If there is a change in sectors it eturns the id of the new sector, with no change returns {0,0}
Vector2i LevelScene::getNewSector() {
	Vector2f plyPos = _player->getPosition();
	// Top border collision
	if (plyPos.y - tileBounds <= topYBorder && _activeSector.y > 1) {
		return Vector2i(_activeSector.x, _activeSector.y - 1);
	}
	// Bottom border collision
	if (plyPos.y + tileBounds >= bottomYBorder && _activeSector.y < 3) {
		return Vector2i(_activeSector.x, _activeSector.y + 1);
	}
	// Left border collision
	if (plyPos.x - tileBounds <= leftXBorder && _activeSector.x > 1) {
		return Vector2i(_activeSector.x - 1, _activeSector.y);
	}
	// Right border collision
	if (plyPos.x + tileBounds >= rightXBorder && _activeSector.x < 3) {
		return Vector2i(_activeSector.x + 1, _activeSector.y);
	}
	return Vector2i(0, 0);
}

// Move player to the other side of the screen simulating continuous movement
void LevelScene::MovePlayerOnNewSector(Vector2i oldS, Vector2i newS) {
	Vector2f newPos = _player->getPosition();
	// Top > down
	if (oldS.y < newS.y) {
		newPos.y -= sectorBounds.y;
	}
	// Bottom > up
	else if (oldS.y > newS.y) {
		newPos.y += sectorBounds.y;
	}
	// Left > right
	else if (oldS.x < newS.x) {
		newPos.x -= sectorBounds.x;
	}
	// Right > left
	else if (oldS.x > newS.x) {
		newPos.x += sectorBounds.x;
	}
	_player->setPosition(newPos);
}