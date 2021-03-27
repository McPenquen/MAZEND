#include "scene.h"
#include "game.h"


// Scene
void Scene::Update(double dt) {
	_ents.Update(dt);
}

void Scene::Render() {
	_ents.Render();
}

vector<shared_ptr<Entity>>& Scene::getEnts() { return _ents.list; }