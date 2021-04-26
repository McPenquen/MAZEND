#include "level1_scene.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "LevelSystem.h"
#include "../components/cmp_player_movement.h"
#include "../components/cmp_shape.h"
#include "../components/cmp_enemy_movement.h"
#include "../components/cmp_state_machine.h"
#include "../enemy_ai/enemy_states.h"

void Level1Scene::Load() {
	// Load the initial sector and player position
	_activeSector = Vector2i(2, 1);
	_activePlayerFloor = 2;
	_playerCollisionVelue = tileBounds;
	DisplaySector();

	LevelScene::Load("res/maps/lvl1/botMap.txt", "res/maps/lvl1/midMap.txt", "res/maps/lvl1/topMap.txt");

	// Set the time limit
	_timeLimitValue.minutes = 2.0f;
	_timeLimitValue.seconds = 0.0f;

	// Move the player to the starting position in the chosen sector corresponding to the correct floor
	movePlayerTo(Vector2f(_sectorBorders.left + tileBounds * 7.0f, _sectorBorders.top + tileBounds * 15.0f));
	// Set the active player
	setActivePlayer();

	// Create an enemy
	auto en = makeEntity(6);
	en->setNameTag("enemy1");
	en->setCollisionBounds(tileBounds);
	en->setVisible(false);
	auto enS = en->addComponent<ShapeComponent>();
	enS->setShape<RectangleShape>(Vector2f(tileBounds, tileBounds));
	enS->getShape().setFillColor(Color::Red);
	enS->getShape().setOutlineColor(Color::White);
	enS->getShape().setOutlineThickness(2.f);
	enS->getShape().setOrigin(Vector2f(tileBounds/2, tileBounds/2));
	en->setPosition(Vector2f(Engine::GetWindowSize().x / 2 + 100.f, Engine::GetWindowSize().y / 2 + 100.f));
	auto enM = en->addComponent<EnemyMovementComponent>(Vector2i(3, 3), ents.players, _activePlayerFloor - 1);
	enM->setFloor(3);
	enM->setSpeed(50.0f);
	auto enSm = en->addComponent<StateMachineComponent>();
	enSm->addState("casual", make_shared<CasualState>(_activePlayer));
	enSm->addState("hunting", make_shared<HuntingState>(_activePlayer));
	enSm->changeState("casual");

	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"ESC for PAUSE"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("level1");

	setLoaded(true);
}

void Level1Scene::Update(const double dt) {
	LevelScene::Update(dt);
}

void Level1Scene::DisplaySector() {
	auto txt = makeEntity(1);
	txt->setPosition(Vector2f((Engine::GetWindowSize().x / 2) - 80,0));
	string str = "Sector " + to_string(_activeSector.x) + ", " + to_string(_activeSector.y);
	auto t = txt->addComponent<TextComponent>(str);
}
