#include "cmp_collectables.h"
#include "LevelSystem.h"
#include "maths.h"
#include "cmp_player_movement.h"

CollectableComponent::CollectableComponent(Entity* p) : Component(p) {}

void CollectableComponent::Update(double dt) 
{
	if(			)
	{

	}
}

float CollectableComponent::getPoints() {
	return _points;
}

void CollectableComponent::setPoints(float new_points) {
	_points = new_points;
}