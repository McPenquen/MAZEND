#pragma once
#include "engine.h"
#include "level_scene.h"

class Level2Scene : public LevelScene {
public:
	Level2Scene() = default;
	~Level2Scene() = default;

	void Load() override;
	void Update(const double dt) override;
};