#include <SFML/Graphics.hpp>
#include "game.h"
#include "../engine/engine.h"

// Menu
MainMenuScene mainMenu;
OptionsScene options;
LevelsScene levels;
PauseMenuScene pauseMenu;

// Levels
Level1Scene level1;
//Scene level2;
//Scene level3;

// Game
GameOverScene gameOverScn;

int main(){
	Engine::Start(gameWidth, gameHeight, "MAZEND", &mainMenu);
	return 0;
}