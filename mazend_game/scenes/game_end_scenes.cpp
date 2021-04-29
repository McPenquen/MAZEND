#include "game_end_scenes.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_shape.h"
#include "../components/cmp_random_movement.h"

// Game Over
void GameOverScene::Load() {
	// Create text GameOver
	auto txt = makeEntity("");
	txt->setPosition(Vector2f(Engine::GetWindowSize().x/2 - 100, Engine::GetWindowSize().y/2 - 100));
	auto t = txt->addComponent<TextComponent>(
		"GAME OVER\n\nPress ESC"
		);

	// Create entities to move around
	for (int i = 0; i < 3; ++i) {
		auto en = makeEntity("");
		auto rad = tileBounds / (i + 1);
		en->setCollisionBounds(rad);
		auto enS = en->addComponent<ShapeComponent>();
		enS->setShape<CircleShape>(rad);
		enS->getShape().setFillColor({ 222, 120, 31 });
		enS->getShape().setOutlineColor(Color::Black);
		enS->getShape().setOutlineThickness(2.f);
		enS->getShape().setOrigin(Vector2f(rad, rad));
		en->setPosition(Vector2f(100.0f + i * 300.0f, Engine::GetWindowSize().y / 2 + i * 150.0f));
		auto enM = en->addComponent<RandomMovementComponent>();
	}

	setSceneName("gameOverScene");
	setLoaded(true);
}

void GameOverScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}

// Victory
void VictoryScene::Load() {
	TimeLimit tl = LevelScene::getTimeLimit();
	string scoreStr = to_string(int(tl.minutes)) + "." + to_string(int(tl.seconds));
	string txtString = "VICTORY\n\nScore: " + scoreStr + "\n\nPress ESC";
	
	// Save the score if it is higher
	bool isHighScore = Engine::SaveScore(LevelScene::getCurrentLevel(), scoreStr);

	// Display the score
	auto txt = makeEntity("");
	txt->setPosition(Vector2f(Engine::GetWindowSize().x / 2 - 100, Engine::GetWindowSize().y / 2 - 100));
	auto t = txt->addComponent<TextComponent>(txtString);

	// Create entities to move around
	for (int i = 0; i < 3; ++i) {
		auto en = makeEntity("");
		auto rad = tileBounds / (i + 1);
		en->setCollisionBounds(rad);
		auto enS = en->addComponent<ShapeComponent>();
		enS->setShape<CircleShape>(rad);
		enS->getShape().setFillColor({ 222, 120, 31 });
		enS->getShape().setOutlineColor(Color::Black);
		enS->getShape().setOutlineThickness(2.f);
		enS->getShape().setOrigin(Vector2f(rad, rad));
		en->setPosition(Vector2f(100.0f + i * 300.0f, Engine::GetWindowSize().y / 2 + i * 150.0f));
		auto enM = en->addComponent<RandomMovementComponent>();
	}

	setSceneName("victoryScene");
	setLoaded(true);
}

void VictoryScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}