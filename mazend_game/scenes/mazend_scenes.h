#pragma once
#include "scene.h"

extern shared_ptr<Scene> gameScene;
extern shared_ptr<Scene> mainMenuScene;
extern shared_ptr<Scene> optionsScene;
extern shared_ptr<Scene> pauseMenuScene;
extern shared_ptr<Scene> levelsScene;
extern shared_ptr<Scene> activeScene;


// Main Menu Scene
class MainMenuScene : public Scene {
private:
	Text text;
	Font font;
public:
	MainMenuScene() = default;
	void Update(double dt) override;
	void Render() override;
	void Load() override;
};

// Options Scene
class OptionsScene : public Scene {
private:
	Text text;
	Font font;
public:
	OptionsScene() = default;
	void Update(double dt) override;
	void Render() override;
	void Load() override;
};

// Levels Scene
class LevelsScene : public Scene {
private:
	Text text;
	Font font;
public:
	LevelsScene() = default;
	void Update(double dt) override;
	void Render() override;
	void Load() override;
};

// Pause Menu Scene
class PauseMenuScene : public Scene {
private:
	Text text;
	Font font;
public:
	PauseMenuScene() = default;
	void Update(double dt) override;
	void Render() override;
	void Load() override;
};

// Game Scene
class GameScene : public Scene {
private:
	Text text;
	Font font;
public:
	GameScene() = default;
	void Update(double dt) override;
	void Render() override;
	void Load() override;
};