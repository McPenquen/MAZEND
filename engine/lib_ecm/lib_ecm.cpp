#include "lib_ecm.h"
#include "../engine.h"
#include "LevelSystem.h"

// Entity
Entity::Entity(int orderNum) : _orderNum(orderNum), _alive(true), _visible(true), _rotation(1.f) {}

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
	if (_visible && _alive) {
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

void Entity::setNameTag(const string s) {
	_nameTag = s;
}

string Entity::getNameTag() {
	return _nameTag;
}

// Component
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

// EntityManager
void EntityManager::Render(Vector2i sectorID) {
	LS::Render(Engine::GetWindow(), 1, sectorID);
	if (players.size() > 0) {
		players[0]->Render();
	}
	LS::Render(Engine::GetWindow(), 2, sectorID);
	if (players.size() > 1) {
		players[1]->Render();
	}
	LS::Render(Engine::GetWindow(), 3, sectorID);
	if (players.size() > 2) {
		players[2]->Render();
	}
	for (const auto& e : collectables) {
		e->Render();
	}
	for (const auto& e : enemies) {
		e->Render();
	}
	for (const auto& e : ui_list) {
		e->Render();
	}
}

void EntityManager::Update(double dt) {
	for (auto& e : ui_list) {
		e->Update(dt);
	}
	for (auto& e : collectables) {
		e->Update(dt);
	}
	for (auto& e : players) {
		e->Update(dt);
	}
	for (auto& e : enemies) {
		e->Update(dt);
	}
}