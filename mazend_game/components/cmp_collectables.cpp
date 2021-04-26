#include "cmp_collectables.h"
#include "engine.h"
#include "maths.h"
#include "cmp_player_movement.h"
#include "../scenes/level_scene.h"

CollectableComponent::CollectableComponent(Entity* c) : Component(c) {}

void CollectableComponent::Update(double dt)	
{

}
void CollectableComponent::PickUp(Entity* p)
{
	if (length(p->getPosition() - _parent->getPosition()) <= 5.f && _parent->isVisible())
	{
		cout << "You've collected" << endl;
		_parent->setVisible(false);
	}
}

float CollectableComponent::getPoints() {
	return _points;
}

void CollectableComponent::setPoints(float new_points) {
	_points = new_points;
}