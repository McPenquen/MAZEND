#include "level1_scene.h"
#include "../components/cmp_shape.h"
#include "../game.h"

void LevelScene::Load() {
	// Create the mid sector
	auto sector = makeEntity();
	auto ss = sector->addComponent<ShapeComponent>();
	ss->setShape<RectangleShape>(sectorBounds);
	ss->getShape().setFillColor(Color::Black);
	ss->getShape().setOrigin(sectorBounds/2.f);
	ss->getShape().setOutlineColor(Color::White);
	ss->getShape().setOutlineThickness(5.f);
	sector->setPosition(Vector2f(gameWidth / 2, gameHeight / 2));
}