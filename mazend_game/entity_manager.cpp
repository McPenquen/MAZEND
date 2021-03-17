#include "entity_manager.h"

using namespace std;
using namespace sf;

// EntityManager
void EntityManager::Render() {
	for (const auto& e : list) {
		e->Render();
	}
}

void EntityManager::Update(double dt) {
	for (auto& e : list) {
		e->Update(dt);
	}
}