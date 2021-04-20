#pragma once
#include "engine.h"

class MovementComponent : public Component {
protected:
	virtual bool validMove(const Vector2f&);
	float _speed;
	int _floorNum;
	Vector2i _sectorId;

public:
	explicit MovementComponent(Entity* p);
	MovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float new_speed);

	int getFloor() const { return _floorNum; }
	void setFloor(int newFloor) { _floorNum = newFloor; }

	Vector2i getSector() const { return _sectorId; }
	void setSector(Vector2i newSector) { _sectorId = newSector; }

	void move(const Vector2f&);
	void move(float x, float y);

	void Render() override {}
	void Update(double dt) override;
};