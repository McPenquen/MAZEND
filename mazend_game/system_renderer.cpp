#include "system_renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static RenderWindow* rw;

void Renderer::Initialise(RenderWindow& r) { rw = &r; }

RenderWindow& Renderer::getWindow() { return *rw; }

void Renderer::Shutdown() {
	while (!sprites.empty()) {
		sprites.pop();
	}
}

void Renderer::Update(const double &) {}

void Renderer::Render() {
	if (rw == nullptr) {
		throw("No render window set...");
	}
	while (!sprites.empty()) {
		rw->draw(*sprites.front());
		sprites.pop();
	}
}

void Renderer::Queue(const Drawable* s) { sprites.push(s); }
