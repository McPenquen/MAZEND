#include <SFML/Graphics.hpp>
#include "system_renderer.h"
#include "game.h"
#include "game_scene.h"

using namespace std;
using namespace sf;

void Load() {
	gameScene.reset(new GameScene());
	mainMenuScene.reset(new MainMenuScene());
	optionsScene.reset(new OptionsScene());
	pauseMenuScene.reset(new PauseMenuScene());
	levelsScene.reset(new LevelsScene());
	gameScene->Load();
	mainMenuScene->Load();
	optionsScene->Load();
	pauseMenuScene->Load();
	levelsScene->Load();
	activeScene = mainMenuScene;
}

void Update(RenderWindow& window) {
	// Get delta time
	static Clock clock;
	double dt = clock.restart().asSeconds();

	activeScene->Update(dt);

	// ESC quit
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
}

void Render(RenderWindow& window) {
	// Render everything for the active scene
	activeScene->Render();
	// Render everything from the Queue
	Renderer::Render();
}

int main(){
	// Create the Window
	RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "MAZEND");
	// Initalise the Renderer
	Renderer::Initialise(window);
	// Load content
	Load();

	// While the window is open run the game
	while (window.isOpen()){
	  window.clear();
	  Update(window);
	  Render(window);
	  window.display();

	}
	return 0;
}