#include "cmp_player_movement.h"
#include "../game.h"
#include "LevelSystem.h"

PlayerMovementComponent::PlayerMovementComponent(Entity* p, Vector2i sectorID) : MovementComponent(p) { 
	_sectorId = sectorID; 
	_currentSectorBorders = Engine::GetCentreSectorBorders();
	_currentControls = Engine::GetControls();
}

void PlayerMovementComponent::Update(double dt) {
	if (_parent->isAlive()) {
		Vector2f direction = { 0.f, 0.f };
		Vector2f plPos = _parent->getPosition();
		if (Keyboard::isKeyPressed(_currentControls["left"]) && LS::getTileAt(plPos + Vector2f(- tileBounds, 0), _sectorId, _floorNum) != LS::EMPTY) {
			direction.x = -_speed * dt;
		}
		if (Keyboard::isKeyPressed(_currentControls["right"]) && LS::getTileAt(plPos + Vector2f(tileBounds, 0), _sectorId, _floorNum) != LS::EMPTY) {
			direction.x = _speed * dt;
		}
		if (Keyboard::isKeyPressed(_currentControls["down"]) && LS::getTileAt(plPos + Vector2f(0, tileBounds), _sectorId, _floorNum) != LS::EMPTY) {
			direction.y = _speed * dt;
		}
		if (Keyboard::isKeyPressed(_currentControls["up"]) && LS::getTileAt(plPos + Vector2f(0, -tileBounds), _sectorId, _floorNum) != LS::EMPTY) {
			direction.y = -_speed * dt;
		}

		if (_parent->getPosition().x - tileBounds > _currentSectorBorders.left &&
			_parent->getPosition().x + tileBounds < _currentSectorBorders.right &&
			_parent->getPosition().y - tileBounds > _currentSectorBorders.top &&
			_parent->getPosition().y + tileBounds < _currentSectorBorders.bottom) {
			move(direction);
		}
		else {
			if (_parent->getPosition().x - tileBounds <= _currentSectorBorders.left && direction.x < 0) {
				direction.x = 0;
			}
			else if (_parent->getPosition().x + tileBounds >= _currentSectorBorders.right && direction.x > 0) {
				direction.x = 0;
			}
			if (_parent->getPosition().y - tileBounds <= _currentSectorBorders.top && direction.y < 0) {
				direction.y = 0;
			}
			else if (_parent->getPosition().y + tileBounds >= _currentSectorBorders.bottom && direction.y > 0) {
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