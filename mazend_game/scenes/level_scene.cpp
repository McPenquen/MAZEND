#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"
#include "../components/cmp_player_movement.h"
#include "../components/cmp_text.h"
// The amount of dt allowed between sector switches
#define SECTOR_SWITCH_COUNTER 0.5f

static float secSwitchTimer = 0.0f;

void LevelScene::Load(string const s) {
	auto ho = Engine::GetWindowSize().y - (LS::getHeight() * 40.f);
	LS::SetOffset(Vector2f(0, ho));

	//Load the initial sector
	//TODO: get position of the player to start at
	_activeSector = Vector2i(1, 1);
	DisplaySector();

	// Create the mid sector
	auto sector = makeEntity(4);
	sector->setNameTag("sectorFrame");
	auto ss = sector->addComponent<ShapeComponent>();
	ss->setShape<RectangleShape>(sectorBounds);
	ss->getShape().setFillColor(Color::Transparent);
	ss->getShape().setOrigin(sectorBounds / 2.f);
	ss->getShape().setOutlineColor(Color::White);
	ss->getShape().setOutlineThickness(5.f);
	sector->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));

	// Load the tiles
	LS::loadLevelFile(s, 40.0f);

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

	// Create black frame
	auto frame1 = makeEntity(4);
	auto sf1 = frame1->addComponent<ShapeComponent>();
	sf1->setShape<RectangleShape>(Vector2f(tileBounds * 2, sectorBounds.y));
	sf1->getShape().setFillColor(Color::Black);
	sf1->getShape().setOrigin(Vector2f(tileBounds, sectorBounds.y/2));
	frame1->setPosition(Vector2f((gameWidth / 2 - sectorBounds.x / 2 - tileBounds - 5.f), gameHeight / 2));
	auto frame2 = makeEntity(4);
	auto sf2 = frame2->addComponent<ShapeComponent>();
	sf2->setShape<RectangleShape>(Vector2f(tileBounds * 2, sectorBounds.y));
	sf2->getShape().setFillColor(Color::Black);
	sf2->getShape().setOrigin(Vector2f(tileBounds, sectorBounds.y / 2));
	frame2->setPosition(Vector2f((gameWidth / 2 + sectorBounds.x / 2 + tileBounds + 5.f), gameHeight / 2));
	auto frame3 = makeEntity(4);
	auto sf3 = frame3->addComponent<ShapeComponent>();
	sf3->setShape<RectangleShape>(Vector2f(sectorBounds.x, tileBounds * 2));
	sf3->getShape().setFillColor(Color::Black);
	sf3->getShape().setOrigin(Vector2f(sectorBounds.x / 2, tileBounds));
	frame3->setPosition(Vector2f((gameWidth / 2), gameHeight / 2 - sectorBounds.y / 2 - tileBounds - 5.f));
	auto frame4 = makeEntity(4);
	auto sf4 = frame4->addComponent<ShapeComponent>();
	sf4->setShape<RectangleShape>(Vector2f(sectorBounds.x, tileBounds * 2));
	sf4->getShape().setFillColor(Color::Black);
	sf4->getShape().setOrigin(Vector2f(sectorBounds.x / 2, tileBounds));
	frame4->setPosition(Vector2f((gameWidth / 2), gameHeight / 2 + sectorBounds.y / 2 + tileBounds + 5.f));
}

void LevelScene::Render() {
	ents.Render(_activeSector);
}

void LevelScene::Update(double const dt) {
	Scene::Update(dt);
	// Control Sector Switch Motion
	if (secSwitchTimer > 0.0f) { 
		secSwitchTimer -= dt; 
	}
	// If the sector switch timer has reached a value bellow zero 
	// and if the player is colliding with a wall leading to a next sector, a switch is allowed
	Vector2i nv = secSwitchTimer <= 0.0f ? getNewSector() : Vector2i(0, 0);
	if (nv != Vector2i(0, 0)) {
		secSwitchTimer = SECTOR_SWITCH_COUNTER;
		ChangeSector(nv);
	}
}


void LevelScene::UnLoad() {
	cout << "Level Unload" << endl;
	Scene::UnLoad();
	LS::UnLoad();
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
	// Move player to the other side of the square
	MovePlayerOnNewSector(_activeSector, sectorId);
	_activeSector = sectorId;
	DisplaySector();
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
	else if (plyPos.y + tileBounds >= bottomYBorder && _activeSector.y < 3) {
		return Vector2i(_activeSector.x, _activeSector.y + 1);
	}
	// Left border collision
	else if (plyPos.x - tileBounds <= leftXBorder && _activeSector.x > 1) {
		return Vector2i(_activeSector.x - 1, _activeSector.y);
	}
	// Right border collision
	else if (plyPos.x + tileBounds >= rightXBorder && _activeSector.x < 3) {
		return Vector2i(_activeSector.x + 1, _activeSector.y);
	}
	return Vector2i(0, 0);
}

// Move player to the other side of the screen simulating continuous movement
void LevelScene::MovePlayerOnNewSector(Vector2i oldS, Vector2i newS) {
	Vector2f newPos = _player->getPosition();
	// Top > down
	if (oldS.y < newS.y) {
		newPos.y -= sectorBounds.y; // +2 * tileBounds + 2.0f;
	}
	// Bottom > up
	else if (oldS.y > newS.y) {
		newPos.y += sectorBounds.y; // -2 * tileBounds - 2.0f;
	}
	// Left > right
	else if (oldS.x < newS.x) {
		newPos.x -= sectorBounds.x; // +2 * tileBounds + 2.0f;
	}
	// Right > left
	else if (oldS.x > newS.x) {
		newPos.x += sectorBounds.x; // -2 * tileBounds - 2.0f;
	}
	_player->setPosition(newPos);
}