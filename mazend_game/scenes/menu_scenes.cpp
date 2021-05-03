#include "menu_scenes.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_shape.h"
#include "../components/cmp_victory_collectable.h"
#include "../components/cmp_random_movement.h"
#include "SFML/Audio.hpp"
sf::Sound select;
sf::SoundBuffer buffer1;
sf::Music music;

// Main Menu
void MainMenuScene::Load() {

	if (music.getStatus() != sf::SoundSource::Playing)
	{
		music.openFromFile("res/audio/music.wav");
		music.setLoop(true);
		music.setVolume(2);
		music.play();
	}

	buffer1.loadFromFile("res/audio/select.wav");
	select.setBuffer(buffer1);
	select.setVolume(3);
	auto txt = makeEntity("");
	auto t = txt->addComponent<TextComponent>(
		"MAIN MENU\n\nPlay Levels - Press 1\nOptions - Press 2\nExit - Press 3"
		);
	txt->setPosition(Vector2f(2 * tileBounds, 2 * tileBounds));
	auto title = makeEntity("");
	auto tt = title->addComponent<TextComponent>("M\nA\nZ\nE\nN\nD");
	title->setPosition(Vector2f(Engine::GetWindowSize().x /2 + Engine::GetWindowSize().x / 6, 
		Engine::GetWindowSize().y / 8.0f));
	tt->SetColour({22,117,161});
	auto scale = Engine::GetWindowSize().x / 15;
	tt->SetSize(scale);

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

	setSceneName("mainMenu");
	setLoaded(true);
}

void MainMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&levels);
		if (select.getStatus() != sf::SoundSource::Playing)
		{
			select.play();
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&options);
		if (select.getStatus() != sf::SoundSource::Playing)
		{
			select.play();
		}
	}
	Scene::Update(dt);
}

void MainMenuScene::DefaultSetup() {
	// Setup the default controls
	Engine::SetControl("up", Keyboard::Up);
	Engine::SetControl("down", Keyboard::Down);
	Engine::SetControl("left", Keyboard::Left);
	Engine::SetControl("right", Keyboard::Right);
	Engine::SetControl("jump", Keyboard::Space);
}

 // Options
bool OptionsScene::_isChangingControl = false;
string OptionsScene::_changingKeyName = "";

void OptionsScene::Load() {
	auto txt = makeEntity("");
	auto activeControls = Engine::GetControls();

	string menuStr1 = "OPTIONS\n\nWindow Mode:\n   Window Mode (Press W+1) VS Full Screen (Press W+2)\n\Controls:\n   MOVE UP: ";
	string menuStr2 = Engine::Key2String(activeControls["up"]) + " - Press Q+1 for change\n   MOVE DOWN: " + Engine::Key2String(activeControls["down"]) + " - Press Q+2 for change\n   MOVE LEFT: " 
		+ Engine::Key2String(activeControls["left"]) + " - Press Q+3 for change\n   MOVE RIGHT: " + Engine::Key2String(activeControls["right"])
		+ " - Press Q+4 for change\n   JUMP: " + Engine::Key2String(activeControls["jump"]) + " - Press Q+5 for change\n\nBack - Press 9";

	auto t = txt->addComponent<TextComponent>(menuStr1 + menuStr2);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));

	if (_isChangingControl) {
		auto changingMessage = makeEntity("");
		auto cmt = changingMessage->addComponent<TextComponent>("Press a key to be your " + _changingKeyName + " movement\n\t\t\t\tIt cannot be ESC");
		auto winSize = Engine::GetWindowSize();
		changingMessage->setPosition({float(winSize.x) / 5, float(winSize.y) - 100.0f});
	}

	setSceneName("options");
	setLoaded(true);
}

void OptionsScene::Update(const double dt) {
	if (!_isChangingControl) {
		if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::Num1)) {
			Engine::ChangeWindowMode("default");
		}
		if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::Num2)) {
			Engine::ChangeWindowMode("fullscreen");
		}
		if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::Num1)) {
			Engine::SetControl("up", Keyboard::Escape);
			_isChangingControl = true;
			_changingKeyName = "up";
			Engine::ChangeScene(&options);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::Num2)) {
			Engine::SetControl("down", Keyboard::Escape);
			_isChangingControl = true;
			_changingKeyName = "down";
			Engine::ChangeScene(&options);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::Num3)) {
			Engine::SetControl("left", Keyboard::Escape);
			_isChangingControl = true;
			_changingKeyName = "left";
			Engine::ChangeScene(&options);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::Num4)) {
			Engine::SetControl("right", Keyboard::Escape);
			_isChangingControl = true;
			_changingKeyName = "right";
			Engine::ChangeScene(&options);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::Num5)) {
			Engine::SetControl("jump", Keyboard::Escape);
			_isChangingControl = true;
			_changingKeyName = "jump";
			Engine::ChangeScene(&options);
		}
		if (Keyboard::isKeyPressed(Keyboard::Num9)) {
			Engine::ChangeScene(&mainMenu);
			if (select.getStatus() != sf::SoundSource::Playing)
			{
				select.play();
			}
		}
	}
	else {
		if (_changingKeyName != "") {
			Engine::ObserveControlChange(_changingKeyName);
			_changingKeyName = "";
		}
		if (!Engine::isObservingControlChange()) {
			_isChangingControl = false;
			Engine::ChangeScene(&options);
		}
	}
	Scene::Update(dt);
}

// Levels
void LevelsScene::Load() {
	auto title = makeEntity("");
	auto tt = title->addComponent<TextComponent>("M\nA\nZ\nE\nN\nD");
	title->setPosition(Vector2f(Engine::GetWindowSize().x / 2 + Engine::GetWindowSize().x / 6,
		Engine::GetWindowSize().y / 8.0f));
	tt->SetColour({ 22,117,161 });
	auto scale = Engine::GetWindowSize().x / 15;
	tt->SetSize(scale);
	auto txt = makeEntity("");

	// The level should display a score if there is any saved - only one level, level 1
	string score = Engine::GetScore(1);
	score = score == "" ? "" : "(" + score + ")"; // add brackets to the score
	string levelsTxt = "LEVELS\n\nLevel 1 "+ score +" - Press 1\nBack - Press 2";

	auto t = txt->addComponent<TextComponent>(levelsTxt);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));

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

	setSceneName("levels");
	setLoaded(true);
}

void LevelsScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
		if (select.getStatus() != sf::SoundSource::Playing)
		{
			select.play();
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&mainMenu);
		if (select.getStatus() != sf::SoundSource::Playing)
		{
			select.play();
		}
	}
	Scene::Update(dt);
}

// Pause Menu
void PauseMenuScene::Load() {
	auto curtain = makeEntity("");
	auto cs = curtain->addComponent<ShapeComponent>();
	cs->setShape<RectangleShape>(Vector2f(Engine::GetWindowSize().x, Engine::GetWindowSize().y));
	cs->getShape().setFillColor(Color::Black);
	cs->getShape().setOrigin(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	curtain->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	auto title = makeEntity("");
	auto tt = title->addComponent<TextComponent>("M\nA\nZ\nE\nN\nD");
	title->setPosition(Vector2f(Engine::GetWindowSize().x / 2 + Engine::GetWindowSize().x / 6,
		Engine::GetWindowSize().y / 8.0f));
	tt->SetColour({ 22,117,161 });
	auto scale = Engine::GetWindowSize().x / 15;
	tt->SetSize(scale);
	auto txt = makeEntity("");
	auto t = txt->addComponent<TextComponent>(
		"PAUSE\n\nResume - Press 1\nLeave the level - Press 2"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));

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

	setSceneName("pauseMenu");
	setLoaded(true);
}

void PauseMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
		if (select.getStatus() != sf::SoundSource::Playing)
		{
			select.play();
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::UnloadPreviousScene();
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}

