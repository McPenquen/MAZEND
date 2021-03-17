#include "lib_ecm.h"

// Entity
Entity::Entity() {}

Entity::~Entity() {
	for (const auto& c : _components) {
		c->~Component();
	}
}

void Entity::Update(double dt) {
	if (_alive) {
		for (auto& c : _components) {
			c->Update(dt);
		}
	}
	if (_fordeletion) {
		this->~Entity();
	}
}

void Entity::Render() {
	if (_visible) {
		for (const auto& c : _components) {
			c->Render();
		}
	}
}

const Vector2f& Entity::getPosition() const {
	return _position;
}

void Entity::setPosition(const Vector2f& new_position) {
	_position = new_position;
}

float Entity::getRotation() const {
	return _rotation;
}

void Entity::setRotation(float new_rotation) {
	_rotation = new_rotation;
}

bool Entity::isAlive() const {
	return _alive;
}

void Entity::setAlive(bool new_alive) {
	_alive = new_alive;
}

bool Entity::isVisible() const {
	return _visible;
}

void Entity::setVisible(bool new_visible) {
	_visible = new_visible;
}

bool Entity::is_fordeletion() const {
	return _fordeletion;
}

void Entity::setForDelete() {
	_fordeletion = true;
}

// Component
Component::Component() : _parent(nullptr) {}

Component::~Component() {}

bool Component::is_fordeletion() const {
	return _fordeletion;
}

Component::Component(Entity* const p) : _parent(p) {}

void Component::Update(double dt) {
	if (_fordeletion) {
		this->~Component();
	}
}