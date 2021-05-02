#pragma once
#include "engine.h"

class MainMenuScene : public Scene {
public:
	MainMenuScene() = default;
	~MainMenuScene() = default;

	void Load() override;
	void Update(const double dt) override;

	void DefaultSetup() override;
};

class OptionsScene : public Scene {
public:
	OptionsScene() = default;
	~OptionsScene() = default;

	void Load() override;
	void Update(const double dt) override;

private:
	static bool _isChangingControl;
	static string _changingKeyName;
};

class LevelsScene : public Scene {
public:
	LevelsScene() = default;
	~LevelsScene() = default;

	void Load() override;
	void Update(const double dt) override;
private:
};

class PauseMenuScene : public Scene {
public:
	PauseMenuScene() = default;
	~PauseMenuScene() = default;

	void Load() override;
	void Update(const double dt) override;
};
