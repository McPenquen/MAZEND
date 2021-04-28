#include "menu_scenes.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_shape.h"
#include "../components/cmp_victory_collectable.h"

// Main Menu
void MainMenuScene::Load() {
	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"MAIN MENU\n\nPlay Levels - Press 1\nOptions - Press 2\nExit - Press 3"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("mainMenu");
	setLoaded(true);
}

void MainMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&levels);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&options);
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
	auto txt = makeEntity(4);
	auto activeControls = Engine::GetControls();

	string menuStr1 = "OPTIONS\n\nWindow Mode:\n   Window Mode (Press W+1) VS Full Screen (Press W+2)\n\Controls:\n   MOVE UP: ";
	string menuStr2 = Engine::Key2String(activeControls["up"]) + " - Press Q+1 for change\n   MOVE DOWN: " + Engine::Key2String(activeControls["down"]) + " - Press Q+2 for change\n   MOVE LEFT: " 
		+ Engine::Key2String(activeControls["left"]) + " - Press Q+3 for change\n   MOVE RIGHT: " + Engine::Key2String(activeControls["right"])
		+ " - Press Q+4 for change\n   JUMP: " + Engine::Key2String(activeControls["jump"]) + " - Press Q+5 for change\n\nBack - Press 9";

	auto t = txt->addComponent<TextComponent>(menuStr1 + menuStr2);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));

	if (_isChangingControl) {
		auto changingMessage = makeEntity(4);
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
	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"LEVELS\n\nLevel 1 - Press 1\nBack - Press 2"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("levels");
	setLoaded(true);
}

void LevelsScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene(&mainMenu);
	}
	Scene::Update(dt);
}

// Pause Menu
void PauseMenuScene::Load() {
	auto curtain = makeEntity(4);
	auto cs = curtain->addComponent<ShapeComponent>();
	cs->setShape<RectangleShape>(Vector2f(Engine::GetWindowSize().x, Engine::GetWindowSize().y));
	cs->getShape().setFillColor(Color::Black);
	cs->getShape().setOrigin(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	curtain->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2));
	auto txt = makeEntity(4);
	auto t = txt->addComponent<TextComponent>(
		"PAUSE\n\nResume - Press 1\nLeave the level - Press 2"
		);
	txt->setPosition(Vector2f(2* tileBounds, 2* tileBounds));
	setSceneName("pauseMenu");
	setLoaded(true);
}

void PauseMenuScene::Update(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene(&level1);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::UnloadPreviousScene();
		Engine::ChangeScene(&levels);
	}
	Scene::Update(dt);
}
