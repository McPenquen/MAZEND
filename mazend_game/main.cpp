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
Level2Scene level2;
// Game
GameOverScene gameOverScn;
VictoryScene victoryScn;

int main(){
	Engine::Start(gameWidth, gameHeight, "MAZEND", &mainMenu, 0);
	return 0;
}