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

// Game
GameOverScene gameOverScn;
VictoryScene victoryScn;

int main(){
	bool isFullScreen = Engine::GetWinMode();
	Engine::Start(gameWidth, gameHeight, "MAZEND", &mainMenu, isFullScreen);
	return 0;
}