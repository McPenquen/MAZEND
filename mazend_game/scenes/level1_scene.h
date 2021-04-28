#pragma once
#include "engine.h"
#include "level_scene.h"

class Level1Scene : public LevelScene {
public:
	Level1Scene() = default;
	~Level1Scene() = default;

	void Load() override;
	void Update(const double dt) override;
};