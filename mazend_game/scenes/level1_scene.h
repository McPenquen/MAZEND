#pragma once
#include "engine.h"

class Level1Scene : public Scene {
public:
	Level1Scene() = default;
	~Level1Scene() = default;

	void Load() override;
	void Update(const double dt) override;
};