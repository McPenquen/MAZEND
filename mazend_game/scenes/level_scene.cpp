#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"
#include "../components/cmp_player_movement.h"
#include "../components/cmp_text.h"
// The amount of dt allowed between sector switches
#define TIME_DELAY_COUNTER 0.5f

// Timing management
static float secSwitchTimer = 0.0f;
static float stairSwitchTimer = 0.0f;

void LevelScene::Load(string const s, string const s1, string const s2) {
	LS::SetOffset(Vector2f(leftXBorder, topYBorder));

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
	LS::loadLevelFile(s, 2 * tileBounds); // level 1 file loading
    LS::loadLevelFile(s1, 2 * tileBounds); // level 2 file loading
    LS::loadLevelFile(s2, 2 * tileBounds); // level 3 file loading

	float plRad = tileBounds;
	const Color plColor = { 222, 120, 31 }; // #DE781F

	// Create the player for bottom floor
	auto pl = makeEntity(5);
	pl->setNameTag("player1");
	pl->setCollisionBounds(_playerCollisionVelue);
	auto plS = pl->addComponent<ShapeComponent>();
	plS->setShape<CircleShape>(plRad);
	plS->getShape().setFillColor(plColor);
	plS->getShape().setOutlineColor(Color::Black);
	plS->getShape().setOutlineThickness(2.f);
	plS->getShape().setOrigin(Vector2f(plRad, plRad));
	pl->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));
	auto plM = pl->addComponent<PlayerMovementComponent>(_activeSector);
	plM->setSpeed(500.f);
	plM->setFloor(1);
	_player1 = pl;
	// Create the player for middle floor
	auto pl2 = makeEntity(5);
	pl2->setNameTag("player2");
	pl2->setCollisionBounds(_playerCollisionVelue);
	auto plS2 = pl2->addComponent<ShapeComponent>();
	plS2->setShape<CircleShape>(plRad/2);
	plS2->getShape().setFillColor(plColor);
	plS2->getShape().setOutlineColor(Color::Black);
	plS2->getShape().setOutlineThickness(2.f);
	plS2->getShape().setOrigin(Vector2f(plRad/2, plRad/2));
	pl2->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));
	auto plM2 = pl2->addComponent<PlayerMovementComponent>(_activeSector);
	plM2->setSpeed(500.f);
	plM2->setFloor(2);
	_player2 = pl2;
	// Create the player for top floor
	auto pl3 = makeEntity(5);
	pl3->setNameTag("player3");
	pl3->setCollisionBounds(_playerCollisionVelue);
	auto plS3 = pl3->addComponent<ShapeComponent>();
	plS3->setShape<CircleShape>(plRad/4);
	plS3->getShape().setFillColor(plColor);
	plS3->getShape().setOutlineColor(Color::Black);
	plS3->getShape().setOutlineThickness(2.f);
	plS3->getShape().setOrigin(Vector2f(plRad/4, plRad/4));
	pl3->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));
	auto plM3 = pl3->addComponent<PlayerMovementComponent>(_activeSector);
	plM3->setSpeed(500.f);
	plM3->setFloor(3);
	_player3 = pl3;

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

	// Create a time limit var
	auto timeLim = makeEntity(4);
	timeLim->setPosition(Vector2f((gameWidth / 2) - 30, 100));
	timeLim->setNameTag("timeLimit");
	auto tL = timeLim->addComponent<TextComponent>("");
	_timeLimit = timeLim;
}

void LevelScene::Render() {
	ents.Render(_activeSector);
}

void LevelScene::Update(double const dt) {

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::PauseScene(&pauseMenu);
	}

	Scene::Update(dt);

	// Check if the time limit has reached 0
	if (_timeLimitValue.minutes <= 0.0f && (_timeLimitValue.seconds - dt) <= 0.0f) {
		Engine::ChangeScene(&gameOverScn);
	}

	// Update the time limit
	float tDif = _timeLimitValue.seconds - dt;
	if (tDif >= 0) {
		_timeLimitValue.seconds = tDif;
	}
	else {
		_timeLimitValue.minutes -= 1;
		_timeLimitValue.seconds = 60 + tDif;
	}
	// Set the time limit string to contain the new time value
	auto tLStr = _timeLimit->GetComponents<TextComponent>();
	tLStr[0]->SetText(to_string(int(_timeLimitValue.minutes)) + ":" + to_string(int(_timeLimitValue.seconds)));

	// Control Sector Switch Motion
	if (secSwitchTimer > 0.0f) { 
		secSwitchTimer -= dt; 
	}

	// If the sector switch timer has reached a value bellow zero 
	// and if the player is colliding with a wall leading to a next sector, a switch is allowed
	Vector2i nv = secSwitchTimer <= 0.0f ? getNewSector() : Vector2i(0, 0);
	if (nv != Vector2i(0, 0)) {
		secSwitchTimer = TIME_DELAY_COUNTER;
		ChangeSector(nv);
	}

	// Update Stair switch timer
	if (stairSwitchTimer > 0.0f) {
		stairSwitchTimer -= dt;
	}

	// Check if the player isn't on stairs to change floor
	if (LS::isStairs(LS::getTileAt(_activePlayer->getPosition(), _activeSector, _activePlayerFloor))) {
		if (Keyboard::isKeyPressed(Keyboard::Space) && stairSwitchTimer <= 0.0f) {
			stairSwitchTimer = TIME_DELAY_COUNTER;
			int newFloor = LS::getStairsFloorChnage(_activePlayer->getPosition(), _activeSector, _activePlayerFloor);
			changeFloor(newFloor);
		}
	}
}

void LevelScene::UnLoad() {
	cout << "Level Unload" << endl;
	Scene::UnLoad();
	LS::UnLoad();
}

void LevelScene::DisplaySector() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f((gameWidth / 2) + 50, 100));
	string str = "Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}

void LevelScene::ChangeSector(Vector2i sectorId) {
	// Reset the path tiles and collectables
	UnLoadSector();
	// Move player to the other side of the square
	MovePlayerOnNewSector(_activeSector, sectorId);
	_activeSector = sectorId;

	// Update the sector value in the player
	_player1->GetComponents<PlayerMovementComponent>()[0].get()->setSector(_activeSector);
	_player2->GetComponents<PlayerMovementComponent>()[0].get()->setSector(_activeSector);
	_player3->GetComponents<PlayerMovementComponent>()[0].get()->setSector(_activeSector);

	DisplaySector();
}

void LevelScene::UnLoadSector() {
	ents.floor1_list.clear();
	ents.floor2_list.clear();
	ents.floor3_list.clear();
}

// If there is a change in sectors it eturns the id of the new sector, with no change returns {0,0}
Vector2i LevelScene::getNewSector() const {
	Vector2f plyPos = _activePlayer->getPosition();
	// Top border collision
	if (plyPos.y - _playerCollisionVelue <= topYBorder && _activeSector.y > 1) {
		return Vector2i(_activeSector.x, _activeSector.y - 1);
	}
	// Bottom border collision
	else if (plyPos.y + _playerCollisionVelue >= bottomYBorder && _activeSector.y < 3) {
		return Vector2i(_activeSector.x, _activeSector.y + 1);
	}
	// Left border collision
	else if (plyPos.x - _playerCollisionVelue <= leftXBorder && _activeSector.x > 1) {
		return Vector2i(_activeSector.x - 1, _activeSector.y);
	}
	// Right border collision
	else if (plyPos.x + _playerCollisionVelue >= rightXBorder && _activeSector.x < 3) {
		return Vector2i(_activeSector.x + 1, _activeSector.y);
	}
	return Vector2i(0, 0);
}

// Move player to the other side of the screen simulating continuous movement
void LevelScene::MovePlayerOnNewSector(Vector2i oldS, Vector2i newS) {
	Vector2f newPos = _activePlayer->getPosition();
	// Top > down
	if (oldS.y < newS.y) {
		newPos.y -= sectorBounds.y + 10.0f;
	}
	// Bottom > up
	else if (oldS.y > newS.y) {
		newPos.y += sectorBounds.y - 10.0f;
	}
	// Left > right
	else if (oldS.x < newS.x) {
		newPos.x -= sectorBounds.x + 10.0f;
	}
	// Right > left
	else if (oldS.x > newS.x) {
		newPos.x += sectorBounds.x - 10.0f; 
	}
	movePlayerTo(newPos);
}

void LevelScene::movePlayerTo(Vector2f newPos) {
	_player1->setPosition(newPos);
	_player2->setPosition(newPos);
	_player3->setPosition(newPos);
}

void LevelScene::setActivePlayer() {
	if (_activePlayerFloor == 1) {
		_activePlayer = _player1;
		_player1->setVisible(true);
		_player2->setVisible(false);
		_player3->setVisible(false);
	}
	else if (_activePlayerFloor == 2) {
		_activePlayer = _player2;
		_player2->setVisible(true);
		_player1->setVisible(false);
		_player3->setVisible(false);
	}
	else if (_activePlayerFloor == 3) {
		_activePlayer = _player3;
		_player3->setVisible(true);
		_player1->setVisible(false);
		_player2->setVisible(false);
	}
}

void LevelScene::changeFloor(int newFloor) {
	// Make all player entities to be at the same place
	auto realPos = _activePlayer->getPosition();
	_player1->setPosition(realPos);
	_player2->setPosition(realPos);
	_player3->setPosition(realPos);
	// Update the floor
	_activePlayerFloor = newFloor;
	setActivePlayer();
	_activePlayer->GetComponents<PlayerMovementComponent>()[0].get()->setFloor(newFloor);
}