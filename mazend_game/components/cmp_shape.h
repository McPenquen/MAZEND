#pragma once
#include <SFML/Graphics.hpp>
#include <engine.h>

using namespace std;
using namespace sf;

class ShapeComponent : public Component {
protected:
	shared_ptr<Shape> _shape;

public:
	ShapeComponent();
	explicit ShapeComponent(Entity* p);

	void Update(double dt) override;
	void Render() override;

	Shape& getShape() const;

	template <typename T, typename... Targs>
	void setShape(Targs... params) {
		_shape.reset(new T(params...));
	}
};
