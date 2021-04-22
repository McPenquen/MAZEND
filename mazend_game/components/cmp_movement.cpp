#include "cmp_movement.h"
#include "LevelSystem.h"

void MovementComponent::Update(double dt) {}

MovementComponent::MovementComponent(Entity* p) : _speed(100.0f), _floorNum(3), Component(p) {}

float MovementComponent::getSpeed() const { return _speed; }

void MovementComponent::setSpeed(float new_speed) { _speed = new_speed; }

bool MovementComponent::validMove(const Vector2f& pos) {
	// TODO establish valid movement
	return true;
}

void MovementComponent::move(const Vector2f& p) {
	auto pp = _parent->getPosition() + p;

	if (!validMove({ pp.x, _parent->getPosition().y })) {
		pp.x = _parent->getPosition().x;
	}
	if (!validMove({ _parent->getPosition().x, pp.y })) {
		pp.y = _parent->getPosition().y;
	}

	_parent->setPosition(pp);
}

void MovementComponent::move(float x, float y) {
	move(Vector2f(x, y));
}