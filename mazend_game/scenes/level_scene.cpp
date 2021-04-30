#include <algorithm>
#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"
#include "../components/cmp_player_movement.h"
#include "../components/cmp_text.h"
#include "../components/cmp_enemy_movement.h"
#include "../components/cmp_victory_collectable.h"
// The amount of dt allowed between sector switches
#define TIME_DELAY_COUNTER 0.5f

// Timing management
static float secSwitchTimer = 0.0f;
static float stairSwitchTimer = 0.0f;
map<string, Keyboard::Key> LevelScene::_currentControls;
TimeLimit LevelScene::_timeLimitValue;
bool isFirstSector = true;

int LevelScene::_currentLevel = 0;

void LevelScene::Load(string const s, string const s1, string const s2) {
	// Get the controls
	_currentControls = Engine::GetControls();

	// Reset score
	CollectableComponent::resetCollectedAmount();

	// Set the centre sector dimensions
	Engine::SetCentreSectorSize(Vector2f(tileBounds * 20, tileBounds * 20));

	// Save sector borders
	_sectorBorders = Engine::GetCentreSectorBorders();

	// Set offset
	LS::SetOffset(Vector2f(_sectorBorders.left, _sectorBorders.top));
	// Load the tiles
	LS::loadLevelFile(s, 2 * tileBounds); // level 1 file loading
    LS::loadLevelFile(s1, 2 * tileBounds); // level 2 file loading
    LS::loadLevelFile(s2, 2 * tileBounds); // level 3 file loading

	// Create the mid sector
	auto sector = makeEntity("");
	sector->setNameTag("sectorFrame");
	auto ss = sector->addComponent<ShapeComponent>();
	ss->setShape<RectangleShape>(Engine::GetCentreSectorSize());
	ss->getShape().setFillColor(Color::Transparent);
	ss->getShape().setOrigin(Engine::GetCentreSectorSize() / 2.f);
	ss->getShape().setOutlineColor(Color::White);
	ss->getShape().setOutlineThickness(5.f);
	sector->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));

	float plRad = tileBounds;
	const Color plColor = { 222, 120, 31 }; // #DE781F

	// Create the player for bottom floor
	auto pl = makeEntity("player");
	pl->setNameTag("player1");
	pl->setCollisionBounds(_playerCollisionVelue);
	auto plS = pl->addComponent<ShapeComponent>();
	plS->setShape<CircleShape>(plRad/4);
	plS->getShape().setFillColor(plColor);
	plS->getShape().setOutlineColor(Color::Black);
	plS->getShape().setOutlineThickness(2.f);
	plS->getShape().setOrigin(Vector2f(plRad/4, plRad/4));
	pl->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	auto plM = pl->addComponent<PlayerMovementComponent>(_activeSector);
	plM->setSpeed(500.f);
	plM->setFloor(1);
	// Create the player for middle floor
	auto pl2 = makeEntity("player");
	pl2->setNameTag("player2");
	pl2->setCollisionBounds(_playerCollisionVelue);
	auto plS2 = pl2->addComponent<ShapeComponent>();
	plS2->setShape<CircleShape>(plRad/2);
	plS2->getShape().setFillColor(plColor);
	plS2->getShape().setOutlineColor(Color::Black);
	plS2->getShape().setOutlineThickness(2.f);
	plS2->getShape().setOrigin(Vector2f(plRad/2, plRad/2));
	pl2->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	auto plM2 = pl2->addComponent<PlayerMovementComponent>(_activeSector);
	plM2->setSpeed(500.f);
	plM2->setFloor(2);
	// Create the player for top floor
	auto pl3 = makeEntity("player");
	pl3->setNameTag("player3");
	pl3->setCollisionBounds(_playerCollisionVelue);
	auto plS3 = pl3->addComponent<ShapeComponent>();
	plS3->setShape<CircleShape>(plRad);
	plS3->getShape().setFillColor(plColor);
	plS3->getShape().setOutlineColor(Color::Black);
	plS3->getShape().setOutlineThickness(2.f);
	plS3->getShape().setOrigin(Vector2f(plRad, plRad));
	pl3->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	auto plM3 = pl3->addComponent<PlayerMovementComponent>(_activeSector);
	plM3->setSpeed(500.f);
	plM3->setFloor(3);

	// Create black frame
	auto frame1 = makeEntity("");
	auto sf1 = frame1->addComponent<ShapeComponent>();
	sf1->setShape<RectangleShape>(Vector2f(tileBounds * 2, Engine::GetCentreSectorSize().y));
	sf1->getShape().setFillColor(Color::Black);
	sf1->getShape().setOrigin(Vector2f(tileBounds, Engine::GetCentreSectorSize().y/2));
	frame1->setPosition(Vector2f((Engine::GetWindowSize().x / 2 - Engine::GetCentreSectorSize().x / 2 - tileBounds - 5.f), Engine::GetWindowSize().y / 2));
	auto frame2 = makeEntity("");
	auto sf2 = frame2->addComponent<ShapeComponent>();
	sf2->setShape<RectangleShape>(Vector2f(tileBounds * 2, Engine::GetCentreSectorSize().y));
	sf2->getShape().setFillColor(Color::Black);
	sf2->getShape().setOrigin(Vector2f(tileBounds, Engine::GetCentreSectorSize().y / 2));
	frame2->setPosition(Vector2f((Engine::GetWindowSize().x / 2 + Engine::GetCentreSectorSize().x / 2 + tileBounds + 5.f), Engine::GetWindowSize().y / 2));
	auto frame3 = makeEntity("");
	auto sf3 = frame3->addComponent<ShapeComponent>();
	sf3->setShape<RectangleShape>(Vector2f(Engine::GetCentreSectorSize().x, tileBounds * 2));
	sf3->getShape().setFillColor(Color::Black);
	sf3->getShape().setOrigin(Vector2f(Engine::GetCentreSectorSize().x / 2, tileBounds));
	frame3->setPosition(Vector2f((Engine::GetWindowSize().x / 2), Engine::GetWindowSize().y / 2 - Engine::GetCentreSectorSize().y / 2 - tileBounds - 5.f));
	auto frame4 = makeEntity("");
	auto sf4 = frame4->addComponent<ShapeComponent>();
	sf4->setShape<RectangleShape>(Vector2f(Engine::GetCentreSectorSize().x, tileBounds * 2));
	sf4->getShape().setFillColor(Color::Black);
	sf4->getShape().setOrigin(Vector2f(Engine::GetCentreSectorSize().x / 2, tileBounds));
	frame4->setPosition(Vector2f((Engine::GetWindowSize().x / 2), Engine::GetWindowSize().y / 2 + Engine::GetCentreSectorSize().y / 2 + tileBounds + 5.f));

	// Create a time limit var
	auto timeLim = makeEntity("");
	timeLim->setPosition(Vector2f((Engine::GetWindowSize().x / 2) - 30, 100));
	timeLim->setNameTag("timeLimit");
	auto tL = timeLim->addComponent<TextComponent>("");
	_timeLimit = timeLim;

	// Create score
	auto scoreTxt = makeEntity("");
	scoreTxt->setPosition(Vector2f((Engine::GetWindowSize().x / 2) - 30, _sectorBorders.bottom + 10));
	scoreTxt->setNameTag("score");
	auto sT = scoreTxt->addComponent<TextComponent>("");
	_scoreEnt = scoreTxt;

	// Score icon
	auto coin = makeEntity("");
	coin->setNameTag("coinIcon");
	coin->setPosition(Vector2f((Engine::GetWindowSize().x / 2) - 50, _sectorBorders.bottom + 28));
	auto coinS = coin->addComponent<ShapeComponent>();
	coinS->setShape<CircleShape>(tileBounds / 2);
	coinS->getShape().setOrigin(Vector2f(tileBounds / 2, tileBounds / 2));
	coinS->getShape().setFillColor(Color::Yellow);
	coinS->getShape().setOutlineColor({ 222, 120, 31 });
	coinS->getShape().setOutlineThickness(2.f);

	// Create instructions
	auto instructions = makeEntity("temp");
	instructions->setNameTag("instructions");
	instructions->setPosition(Vector2f((Engine::GetWindowSize().x / 2) - 550, (Engine::GetWindowSize().y - 150)));
	auto instrT = instructions->addComponent<TextComponent>("\t\t\t\t\tMove by using " + Engine::Key2String(_currentControls["up"]) + ", " + 
		Engine::Key2String(_currentControls["down"]) + ", " + Engine::Key2String(_currentControls["left"]) + ", " + Engine::Key2String(_currentControls["right"]) + "\nClimb down and up a staircase and jump down one floor\n"+ "\t\t\t\t\t\t\t\t\t\tby " + Engine::Key2String(_currentControls["jump"]) +
		"\nCollect all coins and avoid enemies before the time runs out" 
		);
}

void LevelScene::Render() {
	ents.Render(_activeSector);
}

void LevelScene::Update(double const dt) {

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::PauseScene(&pauseMenu);
	}

	Scene::Update(dt);

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
	if (Keyboard::isKeyPressed(_currentControls["jump"]) && stairSwitchTimer <= 0.0f) {
		if (LS::isStairs(LS::getTileAt(_activePlayer->getPosition(), _activeSector, _activePlayerFloor))) {
			stairSwitchTimer = TIME_DELAY_COUNTER;
			int newFloor = LS::getStairsFloorChnage(_activePlayer->getPosition(), _activeSector, _activePlayerFloor);
			changeFloor(newFloor);
		}
	}

	// Check if the player wants to jump down a floor in up direction- not possible if on the bottom floor
	if (Keyboard::isKeyPressed(_currentControls["jump"]) && Keyboard::isKeyPressed(_currentControls["up"]) && _activePlayerFloor > 1) {
		Vector2f nextPos = _activePlayer->getPosition() + Vector2f(0, -(tileBounds * 2));
		if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor) == LS::EMPTY) {
			// if the floor bellow has a floor there player can jump
			if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor - 1) != LS::EMPTY) {
				movePlayerTo(nextPos);
				changeFloor(_activePlayerFloor - 1);
			} 
		}
	}
	// Check if the player wants to jump down a floor in down direction- not possible if on the bottom floor
	if (Keyboard::isKeyPressed(_currentControls["jump"]) && Keyboard::isKeyPressed(_currentControls["down"]) && _activePlayerFloor > 1) {
		Vector2f nextPos = _activePlayer->getPosition() + Vector2f(0, (tileBounds * 2));
		if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor) == LS::EMPTY) {
			// if the floor bellow has a floor there player can jump
			if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor - 1) != LS::EMPTY) {
				movePlayerTo(nextPos);
				changeFloor(_activePlayerFloor - 1);
			}
		}
	}
	// Check if the player wants to jump down a floor in left direction- not possible if on the bottom floor
	if (Keyboard::isKeyPressed(_currentControls["jump"]) && Keyboard::isKeyPressed(_currentControls["left"]) && _activePlayerFloor > 1) {
		Vector2f nextPos = _activePlayer->getPosition() + Vector2f(-(tileBounds * 2), 0);
		if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor) == LS::EMPTY) {
			// if the floor bellow has a floor there player can jump
			if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor - 1) != LS::EMPTY) {
				movePlayerTo(nextPos);
				changeFloor(_activePlayerFloor - 1);
			}
		}
	}
	// Check if the player wants to jump down a floor in right direction- not possible if on the bottom floor
	if (Keyboard::isKeyPressed(_currentControls["jump"]) && Keyboard::isKeyPressed(_currentControls["right"]) && _activePlayerFloor > 1) {
		Vector2f nextPos = _activePlayer->getPosition() + Vector2f((tileBounds * 2), 0);
		if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor) == LS::EMPTY) {
			// if the floor bellow has a floor there player can jump
			if (LS::getTileAt(nextPos, _activeSector, _activePlayerFloor - 1) != LS::EMPTY) {
				movePlayerTo(nextPos);
				changeFloor(_activePlayerFloor - 1);
			}
		}
	}

	// Check if the enemy has caught the player
	if (ents.enemies.size() > 0) {
		for (const auto &e : ents.enemies) {
			if (_activeSector == e->GetComponents<EnemyMovementComponent>()[0]->getSector()) {
				if (length(_activePlayer->getPosition() - e->getPosition()) <= tileBounds * 1.5f) {
					Engine::ChangeScene(&gameOverScn);
					break;
				}
			}
		}
	}

	// Update score
	_score = CollectableComponent::getCollectedAmount();

	// Set the score to contain a new val
	auto scoreStr = _scoreEnt->GetComponents<TextComponent>();
	scoreStr[0]->SetText(to_string(_score) + "/" + to_string(ents.collectables.size()));

	// Check if the player has collected all collectables
	if (_score == ents.collectables.size()) {
		Engine::ChangeScene(&victoryScn);
	}

	// Check if the time limit has reached 0
	if (_timeLimitValue.minutes <= 0.0f && (_timeLimitValue.seconds - dt) <= 0.0f) {
		Engine::ChangeScene(&gameOverScn);
	}
}

void LevelScene::UnLoad() {
	Scene::UnLoad();
	LS::UnLoad();
}

void LevelScene::ChangeSector(Vector2i sectorId) {
	// Empty the instructions from the entity manager after the first sector
	if (isFirstSector) {
		isFirstSector = false;
		ents.temp_list.clear();
	}

	// Move player to the other side of the square
	MovePlayerOnNewSector(_activeSector, sectorId);
	_activeSector = sectorId;

	// Update the sector value in the player
	for (auto& p : ents.players) {
		p->GetComponents<PlayerMovementComponent>()[0].get()->setSector(_activeSector);
	}
}

// If there is a change in sectors it eturns the id of the new sector, with no change returns {0,0}
Vector2i LevelScene::getNewSector() const {
	Vector2f plyPos = _activePlayer->getPosition();
	// Top border collision
	if (plyPos.y - _playerCollisionVelue <= _sectorBorders.top && _activeSector.y > 1) {
		return Vector2i(_activeSector.x, _activeSector.y - 1);
	}
	// Bottom border collision
	else if (plyPos.y + _playerCollisionVelue >= _sectorBorders.bottom && _activeSector.y < 3) {
		return Vector2i(_activeSector.x, _activeSector.y + 1);
	}
	// Left border collision
	else if (plyPos.x - _playerCollisionVelue <= _sectorBorders.left && _activeSector.x > 1) {
		return Vector2i(_activeSector.x - 1, _activeSector.y);
	}
	// Right border collision
	else if (plyPos.x + _playerCollisionVelue >= _sectorBorders.right && _activeSector.x < 3) {
		return Vector2i(_activeSector.x + 1, _activeSector.y);
	}
	return Vector2i(0, 0);
}

// Move player to the other side of the screen simulating continuous movement
void LevelScene::MovePlayerOnNewSector(Vector2i oldS, Vector2i newS) {
	Vector2f newPos = _activePlayer->getPosition();
	// Top > down
	if (oldS.y < newS.y) {
		newPos.y -= Engine::GetCentreSectorSize().y + 10.0f;
	}
	// Bottom > up
	else if (oldS.y > newS.y) {
		newPos.y += Engine::GetCentreSectorSize().y - 10.0f;
	}
	// Left > right
	else if (oldS.x < newS.x) {
		newPos.x -= Engine::GetCentreSectorSize().x + 10.0f;
	}
	// Right > left
	else if (oldS.x > newS.x) {
		newPos.x += Engine::GetCentreSectorSize().x - 10.0f; 
	}
	movePlayerTo(newPos);
}

void LevelScene::movePlayerTo(Vector2f newPos) {
	for (auto& p : ents.players) {
		p->setPosition(newPos);
	}
}

void LevelScene::setActivePlayer() {
	// Change the active player for the enemies too
	if (ents.enemies.size() > 0) {
		for (auto& e : ents.enemies) {
			e->GetComponents<EnemyMovementComponent>()[0]->changePlayerToHunt(_activePlayerFloor - 1);
		}
	}
	// Change the active player for the collectables
	if (ents.collectables.size() > 0) {
		for (auto& e : ents.collectables) {
			e->GetComponents<CollectableComponent>()[0]->ChangeActivePlayerId(_activePlayerFloor - 1);
		}
	}
	if (_activePlayerFloor == 1) {
		_activePlayer = ents.players[0];
		ents.players[0]->setVisible(true);
		ents.players[1]->setVisible(false);
		ents.players[2]->setVisible(false);
	}
	else if (_activePlayerFloor == 2) {
		_activePlayer = ents.players[1];
		ents.players[1]->setVisible(true);
		ents.players[0]->setVisible(false);
		ents.players[2]->setVisible(false);
	}
	else if (_activePlayerFloor == 3) {
		_activePlayer = ents.players[2];
		ents.players[2]->setVisible(true);
		ents.players[0]->setVisible(false);
		ents.players[1]->setVisible(false);
	}
}

void LevelScene::changeFloor(int newFloor) {
	// Make all player entities to be at the same place
	auto realPos = _activePlayer->getPosition();
	movePlayerTo(realPos);
	// Update the floor
	_activePlayerFloor = newFloor;
	setActivePlayer();
	_activePlayer->GetComponents<PlayerMovementComponent>()[0].get()->setFloor(newFloor);
}

TimeLimit LevelScene::getTimeLimit() {
	return _timeLimitValue;
}