#include <SFML/Graphics.hpp>
#include "system_renderer.h"

using namespace std;
using namespace sf;

void Load() {

}

void Update(RenderWindow& window) {
	// Get delta time
	static Clock clock;
	double dt = clock.restart().asSeconds();

	//activeScene->Update(dt);

	// ESC quit
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
}

void Render(RenderWindow& window) {
	// Render everything for the active scene
	//activeScene->Render();
	// Render everything from the Queue
	Renderer::Render();
}

int main(){
	// Create the Window
	RenderWindow window(sf::VideoMode(1200, 700), "MAZEND");
	// Initalise the Renderer
	Renderer::Initialise(window);

	// While the window is open run the game
	while (window.isOpen()){
	  window.clear();
	  Update(window);
	  Render(window);
	  window.display();

	}
	return 0;
}