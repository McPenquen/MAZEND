#pragma once
#include "engine.h"

class LevelScene : public Scene {
public:
	LevelScene() = default;
	~LevelScene() = default;
	void Load() override;
protected:
	shared_ptr<Entity> player;
};