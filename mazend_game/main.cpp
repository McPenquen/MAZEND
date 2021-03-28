#include <SFML/Graphics.hpp>
#include "game.h"
#include "../engine/engine.h"

// Menu
Scene mainMenu;
Scene options;
Scene levels;
Scene pauseMenu;

// Levels
Scene level1;
Scene level2;
Scene level3;

int main(){
	Engine::Start(gameWidth, gameHeight, "MAZEND", &mainMenu);
	return 0;
}