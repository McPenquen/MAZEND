#pragma once
#include "engine.h"
#include "level_scene.h"

class GameOverScene : public Scene {
public:
	GameOverScene() = default;
	~GameOverScene() = default;

	void Load() override;
	void Update(const double dt) override;
};
