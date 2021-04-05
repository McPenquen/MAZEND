#include "cmp_shape.h"
#include <system_renderer.h>

void ShapeComponent::Update(double dt) {
	_shape->setPosition(_parent->getPosition());
}

void ShapeComponent::Render() {
	Renderer::Queue(_shape.get());
}

Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(make_shared<CircleShape>()) {}
