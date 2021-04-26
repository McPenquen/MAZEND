#pragma once
#include <SFML/Graphics.hpp>
#include "scenes/menu_scenes.h"
#include "scenes/level1_scene.h"
#include "LevelSystem.h"
#include "scenes/game_end_scenes.h"

// Window specs
constexpr size_t gameWidth = 1200;
constexpr size_t gameHeight = 700;

// SCENES
extern MainMenuScene mainMenu;
extern OptionsScene options;
extern LevelsScene levels;
extern PauseMenuScene pauseMenu;
extern GameOverScene gameOverScn;
extern Level1Scene level1;
//extern Scene level2;
//extern Scene level3;

// tile size
static float tileBounds = 20.f;
