#pragma once
#include <SFML/Graphics.hpp>
#include <lib_ecm.h>

using namespace std;
using namespace sf;

class CollectableComponent : public Component {
protected:
	float _points = 1.f;

public:
	explicit CollectableComponent(Entity* p);
	CollectableComponent() = delete;
	~CollectableComponent() = default;

	float getPoints();
	void setPoints(float new_points);

	void Render() {}
	void Update(double dt) override;
};