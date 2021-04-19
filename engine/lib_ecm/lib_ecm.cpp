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
	for (const auto& e : floor1_list) {
		e->Render();
	}
	LS::Render(Engine::GetWindow(), 2, sectorID);
	for (const auto& e : floor2_list) {
		e->Render();
	}
	LS::Render(Engine::GetWindow(), 3, sectorID);
	for (const auto& e : floor3_list) {
		e->Render();
	}
	for (const auto& e : floor4_list) {
		e->Render();
	}
}

void EntityManager::Update(double dt) {
	for (auto& e : floor1_list) {
		e->Update(dt);
	}
	for (auto& e : floor2_list) {
		e->Update(dt);
	}
	for (auto& e : floor3_list) {
		e->Update(dt);
	}
	for (auto& e : floor4_list) {
		e->Update(dt);
	}
}

vector<shared_ptr<Entity>> EntityManager::find(const string& tag, int floor) const {
	vector<shared_ptr<Entity>> result;
	if (floor == 1) {
		for (auto& e : floor1_list) {
			if (e->getNameTag() == tag) {
				result.push_back(e);
			}
		}
	}
	else if (floor == 2) {
		for (auto& e : floor2_list) {
			if (e->getNameTag() == tag) {
				result.push_back(e);
			}
		}
	}
	else if (floor == 3) {
		for (auto& e : floor3_list) {
			if (e->getNameTag() == tag) {
				result.push_back(e);
			}
		}
	}
	else {
		for (auto& e : floor4_list) {
			if (e->getNameTag() == tag) {
				result.push_back(e);
			}
		}
	}
	return result;
}

vector<shared_ptr<Entity>> EntityManager::find(const vector<string>& tags, int floor) const {
	vector<shared_ptr<Entity>> result;
	if (floor == 1) {
		for (auto& e : floor1_list) {
			for (auto tag : tags) {
				if (e->getNameTag() == tag) {
					result.push_back(e);
				}
			}
		}
	}
	else if (floor == 2) {
		for (auto& e : floor1_list) {
			for (auto tag : tags) {
				if (e->getNameTag() == tag) {
					result.push_back(e);
				}
			}
		}
	}
	else if (floor == 3){
		for (auto& e : floor3_list) {
			for (auto tag : tags) {
				if (e->getNameTag() == tag) {
					result.push_back(e);
				}
			}
		}
	}
	else {
		for (auto& e : floor4_list) {
			for (auto tag : tags) {
				if (e->getNameTag() == tag) {
					result.push_back(e);
				}
			}
		}
	}
	return result;
}