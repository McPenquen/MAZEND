#include "cmp_player_movement.h"
#include "../game.h"
#include "LevelSystem.h"

PlayerMovementComponent::PlayerMovementComponent(Entity* p, Vector2i sectorID) : MovementComponent(p) { _sectorId = sectorID; }

void PlayerMovementComponent::Update(double dt) {
	if (_parent->isAlive()) {
		Vector2f direction = { 0.f, 0.f };
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			direction.x = -_speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			direction.x = _speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			direction.y = _speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			direction.y = -_speed * dt;
		}

		if (_parent->getPosition().x - tileBounds > leftXBorder &&
			_parent->getPosition().x + tileBounds < rightXBorder &&
			_parent->getPosition().y - tileBounds > topYBorder &&
			_parent->getPosition().y + tileBounds < bottomYBorder) {
			move(direction);
		}
		else {
			if (_parent->getPosition().x - tileBounds <= leftXBorder && direction.x < 0) {
				direction.x = 0;
			}
			else if (_parent->getPosition().x + tileBounds >= rightXBorder && direction.x > 0) {
				direction.x = 0;
			}
			if (_parent->getPosition().y - tileBounds <= topYBorder && direction.y < 0) {
				direction.y = 0;
			}
			else if (_parent->getPosition().y + tileBounds >= bottomYBorder && direction.y > 0) {
				direction.y = 0;
			}
			move(direction);
		}
	}
}

bool PlayerMovementComponent::validMove(const Vector2f& pos) {
	if (!_parent->isVisible()) {
		return true;
	}
	LS::TILE t = LS::getTileAt(pos, _sectorId, _floorNum);
	return t != LS::EMPTY;
}