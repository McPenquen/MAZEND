#pragma once
#include "engine.h"

class MainMenuScene : public Scene {
public:
	MainMenuScene() = default;
	~MainMenuScene() = default;

	void Load() override;
	void Update(const double dt) override;

	void Initialise() override;
};

class OptionsScene : public Scene {
public:
	OptionsScene() = default;
	~OptionsScene() = default;

	void Load() override;
	void Update(const double dt) override;
};

class LevelsScene : public Scene {
public:
	LevelsScene() = default;
	~LevelsScene() = default;

	void Load() override;
	void Update(const double dt) override;
};

class PauseMenuScene : public Scene {
public:
	PauseMenuScene() = default;
	~PauseMenuScene() = default;

	void Load() override;
	void Update(const double dt) override;
};
