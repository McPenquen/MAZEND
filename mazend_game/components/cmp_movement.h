#pragma once
#include "engine.h"

class MovementComponent : public Component {
protected:
	bool validMove(const Vector2f&);
	float _speed;

public:
	explicit MovementComponent(Entity* p);
	MovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float new_speed);

	void move(const Vector2f&);
	void move(float x, float y);

	void Render() override {}
	void Update(double dt) override;
};