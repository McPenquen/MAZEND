#include "game_end_scenes.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../components/cmp_shape.h"
#include "../components/cmp_random_movement.h"
sf::Sound victory;
sf::SoundBuffer buffer4;
sf::Sound Defeat;
sf::SoundBuffer buffer6;
sf::Sound select2;
sf::SoundBuffer buffer5;
// Game Over
void GameOverScene::Load() {
	buffer6.loadFromFile("res/audio/defeat.wav");
	Defeat.setBuffer(buffer6);
	Defeat.setVolume(3);
	Defeat.play();
	buffer5.loadFromFile("res/audio/select.wav");
	select2.setBuffer(buffer5);
	select2.setVolume(3);
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
		if (select2.getStatus() != sf::SoundSource::Playing)
		{
			select2.play();
		}
	}
	Scene::Update(dt);
}

// Victory
void VictoryScene::Load() {
	buffer4.loadFromFile("res/audio/victory.wav");
	victory.setBuffer(buffer4);
	victory.setVolume(5);
	buffer5.loadFromFile("res/audio/select.wav");
	select2.setBuffer(buffer5);
	select2.setVolume(3);
	victory.play();
	TimeLimit tl = LevelScene::getTimeLimit();
	string scoreStr = to_string(int(tl.minutes)) + "." + to_string(int(tl.seconds));
	string txtString = "VICTORY\n\nScore: " + scoreStr;
	
	// Alter the message on the screen depending on if it was a high score or not
	txtString.append(Engine::SaveScore(LevelScene::getCurrentLevel(), scoreStr) ? 
		"\nHigh Score\n\nPress ESC" 
		: "\n\nPress ESC");

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
		if (select2.getStatus() != sf::SoundSource::Playing)
		{
			select2.play();
		}
	}
	Scene::Update(dt);
}