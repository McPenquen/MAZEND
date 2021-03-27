#include "game_scene.h"
#include "game.h"
#include "system_renderer.h"

// Scenes
shared_ptr<Scene> gameScene;
shared_ptr<Scene> mainMenuScene;
shared_ptr<Scene> optionsScene;
shared_ptr<Scene> pauseMenuScene;
shared_ptr<Scene> levelsScene;
shared_ptr<Scene> activeScene;

// Boolean if we are mid game
static bool is_Playing = false;

// Main Menu Scene
void MainMenuScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		activeScene = levelsScene;
	}
	if (Keyboard::isKeyPressed(Keyboard::O)) {
		activeScene = optionsScene;
	}
	Scene::Update(dt);
	text.setString("MAIN MENU\n\n\nPlay Levels - press SPACE\n\nOptions - press O\n\nExit - press ESC");
}

void MainMenuScene::Render() {
	Renderer::Queue(&text);
	Scene::Render();
}

void MainMenuScene::Load() {
	font.loadFromFile("res/fonts/Roboto-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setPosition((gameWidth * .4f) - (text.getLocalBounds().width * .4f), 0);
}

// Options Scene
void OptionsScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		activeScene = is_Playing ? pauseMenuScene : mainMenuScene;
	}
	Scene::Update(dt);
	text.setString("OPTIONS\n\n\nBack - press Q");
}

void OptionsScene::Render() {
	Renderer::Queue(&text);
	Scene::Render();
}

void OptionsScene::Load() {
	font.loadFromFile("res/fonts/Roboto-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setPosition((gameWidth * .4f) - (text.getLocalBounds().width * .4f), 0);
}

// Levels Scene
void LevelsScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		is_Playing = true;
		activeScene = gameScene;
	}
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		activeScene = mainMenuScene;
	}
	Scene::Update(dt);
	text.setString("LEVELS\n\n\nPlay - press SPACE\n\nBack - press Q");
}

void LevelsScene::Render() {
	Renderer::Queue(&text);
	Scene::Render();
}

void LevelsScene::Load() {
	font.loadFromFile("res/fonts/Roboto-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setPosition((gameWidth * .4f) - (text.getLocalBounds().width * .4f), 0);
}

// Pause Menu Scene
void PauseMenuScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		activeScene = gameScene;
	}
	if (Keyboard::isKeyPressed(Keyboard::O)) {
		activeScene = optionsScene;
	}
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		is_Playing = false;
		activeScene = mainMenuScene;
	}
	Scene::Update(dt);
	text.setString("PAUSE MENU\n\n\nResume - press SPACE\n\nOptions - press O\n\nQuit - press Q");
}

void PauseMenuScene::Render() {
	Renderer::Queue(&text);
	Scene::Render();
}

void PauseMenuScene::Load() {
	font.loadFromFile("res/fonts/Roboto-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setPosition((gameWidth * .4f) - (text.getLocalBounds().width * .4f), 0);
}

// Game Scene
void GameScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		activeScene = pauseMenuScene;
	}
	Scene::Update(dt);
	text.setString("GAME\n\n\nPause - press SPACE");
}

void GameScene::Render() {
	Renderer::Queue(&text);
	Scene::Render();
}

void GameScene::Load() {
	font.loadFromFile("res/fonts/Roboto-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setPosition((gameWidth * .4f) - (text.getLocalBounds().width * .4f), 0);
}