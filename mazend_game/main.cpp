#include <SFML/Graphics.hpp>
#include "shlobj_core.h"
#include "game.h"
#include "../engine/engine.h"

// Menu
MainMenuScene mainMenu;
OptionsScene options;
LevelsScene levels;
PauseMenuScene pauseMenu;
CreditsScene credits;

// Levels
Level1Scene level1;

// Game
GameOverScene gameOverScn;
VictoryScene victoryScn;

int main(){
    // Save the location of the database files
    // from: https://stackoverflow.com/questions/35042967/cannot-get-shgetknownfolderpath-function-working
    // & from: https://social.msdn.microsoft.com/Forums/en-US/5674a45e-5548-435e-9fe0-c1a76ae86944/get-path-as-stdstring-using-shgetknownfolderpath?forum=vclanguage
    PWSTR pwstrPath = NULL;
    auto shg = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pwstrPath);
    wstring strPath(pwstrPath);
    string finalString = "";
    for (int i = 0; i < strPath.length(); i++) {
        finalString.push_back(strPath[i]);
    }
    finalString = finalString + "\\Mazend\\";
    Engine::SetDatabaseLocation(finalString);

	bool isFullScreen = Engine::GetWinMode();
	Engine::Start(gameWidth, gameHeight, "MAZEND", &mainMenu, isFullScreen);
	return 0;
}