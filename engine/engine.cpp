#include "engine.h"
#include <chrono>
#include <future>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "system_renderer.h"
#include <maths.h>
#include "../mazend_game/scenes/menu_scenes.h"
#include "LevelSystem.h"
// The amount of dt that scenes will wait in order to siwtch to another scene 
#define sceneSwithTime 0.5

// Scene
void Scene::Update(const double dt) { 
	ents.Update(dt); 
}

void Scene::Render() {
	//Default sector render is {1,1} 
	ents.Render(Vector2i(1,1));
}

bool Scene::isLoaded() const {
	lock_guard<std::mutex> lck(_loaded_mtx);
	if (_loaded_future.valid() && _loaded_future.wait_for(chrono::seconds(0)) == future_status::ready) {
		_loaded_future.get();
		_loaded = true;
	}
	return _loaded;
}

void Scene::setLoaded(bool new_bool) {
	lock_guard<mutex> lck(_loaded_mtx);
	_loaded = new_bool;
}

void Scene::UnLoad() {
	ents.ui_list.clear();
	ents.players.clear();
	ents.enemies.clear();
    ents.collectables.clear();
    ents.temp_list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() {
	_loaded_future = async(&Scene::Load, this);
}

Scene::~Scene() {
	UnLoad();
}

shared_ptr<Entity> Scene::makeEntity(string enName) {
	auto en = make_shared<Entity>();
	if (enName == "player") {
		ents.players.push_back(en);
	}
	else if (enName == "enemy") {
		ents.enemies.push_back(en);
	}
    else if (enName == "collectable") {
		ents.collectables.push_back(en);
	}
    else if (enName == "temp") {
        ents.temp_list.push_back(en);
    }
    // Else it belongs to UI
	else {
		ents.ui_list.push_back(en);
	}
	return move(en);
}

EntityManager Scene::getEnts() {
	return ents;
}

string Scene::getSceneName() const {
	return _sceneName;
}

void Scene::setSceneName(const string s) {
	_sceneName = s;
}

// Engine
Scene* Engine::_activeScene = nullptr;
Scene* Engine::_previousScene = nullptr;
string Engine::_gameName;
Vector2f Engine::_centreSector;
string Engine::_changingMode = "";
map<string, Keyboard::Key> Engine::_controls;
string Engine::_observingControlName = "";
bool Engine::_isObservingControlChange = false;

// - Loading
static bool loading = false;
static float loadingProgress = 0.0f;
static float loadingTime;
static RenderWindow* _window;

void LoadingUpdate(double dt, const Scene* const scene) {
	if (scene->isLoaded()) {
		loading = false;
	}
	else {
		loadingProgress += 220.f * dt;
		loadingTime += dt;
	}
}

static Font loadingFont;

void LoadingRender() {
	static CircleShape octagon(80, 8);
	octagon.setOrigin(80, 80);
	octagon.setRotation(loadingProgress);
	octagon.setPosition(Vcast<float>(Engine::GetWindowSize()) * .5f);
	octagon.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	static Text t;
	t.setString("Loading Level");
	loadingFont.loadFromFile("res/fonts/ZenDots-Regular.ttf");
	t.setFont(loadingFont);
	t.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	Vector2f winPos = Vcast<float>(Engine::GetWindowSize());
	t.setPosition(winPos * Vector2f(0.4f, 0.3f));
	Renderer::Queue(&t);
	Renderer::Queue(&octagon);
}

// - Engine
void Engine::Update() {
	static Clock clock;
	float dt = clock.restart().asSeconds();

	scnSwitchTimer += dt;

	if (loading) {
		LoadingUpdate(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {
	if (loading) {
		LoadingRender();
	}
	else if (_activeScene != nullptr) {
		_activeScene->Render();
	}
	Renderer::Render();
}

void Engine::Start(unsigned int width, unsigned int height, const string& gameName, Scene* scene, bool isFullscreen) {
	// Reset the _changingMode value
	_changingMode = "";

	// If it's the first time running the Start()
	if (_window == nullptr) {
		scene->DefaultSetup();
	}

	RenderWindow window(VideoMode(width, height), gameName, isFullscreen ? Style::Fullscreen : Style::Default);
    UpdateSavedControls();
    _window = &window;
	_gameName = gameName;
	window.setMouseCursorVisible(false);
	Renderer::Initialise(*_window);
	ChangeScene(scene);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
        // Close the game
		if (_activeScene->getSceneName() == "mainMenu" && (Keyboard::isKeyPressed(Keyboard::Num3))) {
			window.close();
		}
        // Change Window mode
		if (_changingMode != "") {
			if (_changingMode == "fullscreen") {
				window.close();
                SaveWinMode(true);
				Engine::Start(width, height, gameName, scene, 1);
			}
			else {
				window.close();
                SaveWinMode(false);
				Engine::Start(width, height, gameName, scene, 0);
			}
		}
        // Save a new Control
        if (_isObservingControlChange) {
            Event event;
            bool flag = true;
            while (flag) {
                while (_window->pollEvent(event)) {
                    if (event.type == Event::KeyPressed && event.key.code != Keyboard::Escape) {
                        _controls[_observingControlName] = event.key.code;
                        _isObservingControlChange = false;
                        _observingControlName = "";
                        flag = false;
                        SaveControls();
                        break;
                    }
                }
            }
        }
		window.clear();
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	if (_previousScene != nullptr) {
		_previousScene->UnLoad();
		_previousScene = nullptr;
	}
	window.close();
}

void Engine::SetVsync(bool bo) {
	_window->setVerticalSyncEnabled(bo);
}

void Engine::ChangeScene(Scene* s) {
	if (scnSwitchTimer >= sceneSwithTime) {
		scnSwitchTimer = 0;
		_previousScene = _activeScene;
		_activeScene = s;

		if (_previousScene != nullptr) {
			_previousScene->UnLoad();
		}

		if (!s->isLoaded()) {
			loadingTime = 0;
			_activeScene->LoadAsync();
			loading = true;
		}
	}
}

void Engine::PauseScene(Scene* s) {
	if (scnSwitchTimer >= sceneSwithTime) {
		scnSwitchTimer = 0;
		_previousScene = _activeScene;
		_activeScene = s;

		if (!s->isLoaded()) {
			loadingTime = 0;
			_activeScene->LoadAsync();
			loading = true;
		}
	}
}

void Engine::UnloadPreviousScene() {
	if (_previousScene != nullptr) {
		_previousScene->UnLoad();
	}
}

Vector2u Engine::GetWindowSize() {
	return _window->getSize();
}

RenderWindow& Engine::GetWindow() {
	return *_window;
}

void Engine::ChangeWindowMode(string modeStr) {
	_changingMode = modeStr;
}

Borders Engine::GetCentreSectorBorders() {
	Borders answer;
	answer.top = _window->getSize().y / 2 - _centreSector.y / 2;
	answer.bottom = _window->getSize().y / 2 + _centreSector.y / 2;
	answer.left = _window->getSize().x / 2 - _centreSector.x / 2;
	answer.right = _window->getSize().x / 2 + _centreSector.x / 2;
	return answer;
}

Vector2f Engine::GetCentreSectorSize() {
	return _centreSector;
}

void Engine::SetCentreSectorSize(Vector2f newSec) {
	_centreSector = newSec;
}

map<string, Keyboard::Key> Engine::GetControls() {
	return _controls;
}

void Engine::SetControl(const string controlName, const Keyboard::Key key) {
    if (_controls.find(controlName) == _controls.end()) {
        _controls.insert({controlName, key});
    }
    else {
        _controls[controlName] = key;
    }
}

void Engine::ObserveControlChange(const string keyName) {
    _observingControlName = keyName;
    _isObservingControlChange = true;
}

bool Engine::isObservingControlChange() {
    return _isObservingControlChange;
}

bool Engine::SaveScore(const int level, const string score) {
    // bool to return
    bool isNewHighScore = false;
    //Load in file
    ifstream fs;
    string buffer;
    fs.open("res/database/scores.txt");
    if (fs.good()) {
        fs.seekg(0, ios::end);
        buffer.resize(fs.tellg());
        fs.seekg(0);
        fs.read(&buffer[0], buffer.size());
        fs.close();
    }
    else {
        throw string("Couldn't open scores");
    }

    // Overwrite the file
    ofstream ofs("res/database/scores.txt", ofstream::trunc);

    // Variables for reading
    string currentString = "";
    bool isListeningFlag = true; // listening to the level id reading
    bool isTaskFinished = false;
    bool isReadingScore = false; // it is reading score now
    string oldScore = "";
    
    // Read all the scores and find the right level
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        // if the level wasn't fo
        if (!isTaskFinished) {
            // if we have detected correct level id, we are reading the score to establish if it's higher
            if (!isListeningFlag && isReadingScore && c != '\n') {
                oldScore.push_back(c);
            }
            if (isListeningFlag) {
                if (c == ',') {
                    if (currentString == to_string(level)) {
                        isReadingScore = true;
                        ofs << c; // add the comma
                    }
                    currentString = "";
                    isListeningFlag = false;
                }
                else {
                    currentString.push_back(c);
                }
            }
            // Listen again after a new line and stop recording score
            else if (c == '\n') {
                isListeningFlag = true;
                if (isReadingScore) {
                    // Compare scores and put the higher one into the database
                    // inspired by: https://stackoverflow.com/questions/4754011/c-string-to-double-conversion
                    double oldScoreVal = atof(oldScore.c_str());
                    double newScoreVal = atof(score.c_str());
                    if (newScoreVal > oldScoreVal) {
                        ofs << score;
                        isNewHighScore = true;
                    }
                    else {
                        ofs << oldScore;
                    }
                    // if we have found the right level the task ends
                    isReadingScore = false;
                    oldScore = "";
                    isTaskFinished = true;
                }
            }
        }
        // push the character to the new version of the file's contents
        if (c != '\0' && !isReadingScore) { // skip \0 characters and don't add the score if we are reading it
            ofs << c;
        }

        // if we are at the end this is the first and so high score
        if (i == buffer.size() - 1 && !isTaskFinished) {
            string newRecord = to_string(level) + "," + score + "\n";
            ofs << newRecord;
            isNewHighScore = true;
        }
    }
    ofs.close();
    return isNewHighScore;
}

string Engine::GetScore(const int level) {
    // Load in file
    ifstream fs;
    string buffer;
    fs.open("res/database/scores.txt");
    if (fs.good()) {
        fs.seekg(0, ios::end);
        buffer.resize(fs.tellg());
        fs.seekg(0);
        fs.read(&buffer[0], buffer.size());
        fs.close();
    }
    else {
        throw string("Couldn't open scores");
    }

    // Variables to control the reading
    bool isWaiting = false; // if the level isn't right don't bother to check the other character until \n
    string currentString = "";
    bool isReadingScore = false;

    // Find the level's score
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        if (!isReadingScore) {
            if (!isWaiting) {
                // After the comma the level part reading has been finished
                if (c == ',') {
                    if (currentString == to_string(level)) {
                        isReadingScore = true;
                    }
                    else {
                        // Wait until \n character
                        isWaiting = true;
                    }
                    // Reset the string
                    currentString = "";
                }
                else {
                    currentString.push_back(c);
                }
            }
            else if (c == '\n') {
                isWaiting = false;
            }
        }
        else { // isReadingScore is true because we have found the right level, now read the score
            if (c == '\n') {
                // After the \n return the score
                return currentString;
            }
            currentString.push_back(c);
        }
    }
    return "";
}

void Engine::SaveControls() {
    // Overwrite the file
    ofstream ofs("res/database/controls.txt", ofstream::trunc);
    for (auto const pair : _controls) {
        string fileLine = pair.first + "," + Key2String(pair.second) + "\n";
        ofs << fileLine;
    }
    ofs.close();
}

void Engine::UpdateSavedControls() {
    // Load in file
    ifstream fs;
    string buffer;
    fs.open("res/database/controls.txt");
    if (fs.good()) {
        fs.seekg(0, ios::end);
        buffer.resize(fs.tellg());
        fs.seekg(0);
        fs.read(&buffer[0], buffer.size());
        fs.close();
    }
    else {
        throw string("Couldn't open controls file");
    }

    // Empty controls
    _controls.clear();

    // Variables for the reading
    string currentString1 = "";
    string currentString2 = "";
    bool isKey = true;

    // Read the file line by line
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        if (c != '\n') {
            if (c == ',') {
                isKey = false; // comma means the value for the map is starting
            }
            else {
                if (isKey) {
                    currentString1.push_back(c);
                }
                else {
                    currentString2.push_back(c);
                }
            }
        }
        else {
            _controls.insert({currentString1, String2Key(currentString2)});

            currentString1 = "";
            currentString2 = "";
            isKey = true;
        }
    }

}

void Engine::SaveWinMode(const bool isFullscreen) {
    // Overwrite the file
    ofstream ofs("res/database/isFullScreen.txt", ofstream::trunc);
    string newText = isFullscreen ? "1" : "0";
    ofs << newText;
    ofs.close();
}

bool Engine::GetWinMode() {
    // Load in file
    ifstream fs;
    string buffer;
    fs.open("res/database/isFullScreen.txt");
    if (fs.good()) {
        fs.seekg(0, ios::end);
        buffer.resize(fs.tellg());
        fs.seekg(0);
        fs.read(&buffer[0], buffer.size());
        fs.close();
    }
    else {
        throw string("Couldn't open fullScreen file");
    }
    char c = buffer[0];
    return c == '1' ? true : false;
}

// from: https://en.sfml-dev.org/forums/index.php?topic=15226.0
// Escape is "?????" - as it shouldn't be an option for controls
string Engine::Key2String(const Keyboard::Key k) {
    string ret;
    switch (k) {

    case Keyboard::A:

        ret = "A";
        break;
    case Keyboard::B:

        ret = "B";
        break;
    case Keyboard::C:

        ret = "C";
        break;
    case Keyboard::D:

        ret = "D";
        break;
    case Keyboard::E:

        ret = "E";
        break;
    case Keyboard::F:

        ret = "F";
        break;
    case Keyboard::G:

        ret = "G";
        break;
    case Keyboard::H:

        ret = "H";
        break;
    case Keyboard::I:

        ret = "I";
        break;
    case Keyboard::J:

        ret = "J";
        break;
    case Keyboard::K:

        ret = "K";
        break;
    case Keyboard::L:

        ret = "L";
        break;
    case Keyboard::M:

        ret = "M";
        break;
    case Keyboard::N:

        ret = "N";
        break;
    case Keyboard::O:

        ret = "O";
        break;
    case Keyboard::P:

        ret = "P";
        break;
    case Keyboard::Q:

        ret = "Q";
        break;
    case Keyboard::R:

        ret = "R";
        break;
    case Keyboard::S:

        ret = "S";
        break;
    case Keyboard::T:

        ret = "T";
        break;
    case Keyboard::U:

        ret = "U";
        break;
    case Keyboard::V:

        ret = "V";
        break;
    case Keyboard::W:

        ret = "W";
        break;
    case Keyboard::X:

        ret = "X";
        break;
    case Keyboard::Y:

        ret = "Y";
        break;
    case Keyboard::Z:

        ret = "Z";
        break;
    case Keyboard::Num0:

        ret = "Num0";
        break;
    case Keyboard::Num1:

        ret = "Num1";
        break;
    case Keyboard::Num2:

        ret = "Num2";
        break;
    case Keyboard::Num3:

        ret = "Num3";
        break;
    case Keyboard::Num4:

        ret = "Num4";
        break;
    case Keyboard::Num5:

        ret = "Num5";
        break;
    case Keyboard::Num6:

        ret = "Num6";
        break;
    case Keyboard::Num7:

        ret = "Num7";
        break;
    case Keyboard::Num8:

        ret = "Num8";
        break;
    case Keyboard::Num9:

        ret = "Num9";
        break;
    case Keyboard::Escape:

        ret = "?????";
        break;
    case Keyboard::LControl:

        ret = "LControl";
        break;
    case Keyboard::LShift:

        ret = "LShift";
        break;
    case Keyboard::LAlt:

        ret = "LAlt";
        break;
    case Keyboard::LSystem:

        ret = "LSystem";
        break;
    case Keyboard::RControl:

        ret = "RControl";
        break;
    case Keyboard::RShift:

        ret = "RShift";
        break;
    case Keyboard::RAlt:

        ret = "RAlt";
        break;
    case Keyboard::RSystem:

        ret = "RSystem";
        break;
    case Keyboard::Menu:

        ret = "Menu";
        break;
    case Keyboard::LBracket:

        ret = "LBracket";
        break;
    case Keyboard::RBracket:

        ret = "RBracket";
        break;
    case Keyboard::SemiColon:

        ret = "SemiColon";
        break;
    case Keyboard::Comma:

        ret = "Comma";
        break;
    case Keyboard::Period:

        ret = "Period";
        break;
    case Keyboard::Quote:

        ret = "Quote";
        break;
    case Keyboard::Slash:

        ret = "Slash";
        break;
    case Keyboard::BackSlash:

        ret = "BackSlash";
        break;
    case Keyboard::Tilde:

        ret = "Tilde";
        break;
    case Keyboard::Equal:

        ret = "Equal";
        break;
    case Keyboard::Dash:

        ret = "Dash";
        break;
    case Keyboard::Space:

        ret = "Space";
        break;
    case Keyboard::Return:

        ret = "Return";
        break;
    case Keyboard::BackSpace:

        ret = "BackSpace";
        break;
    case Keyboard::Tab:

        ret = "Tab";
        break;
    case Keyboard::PageUp:

        ret = "PageUp";
        break;
    case Keyboard::PageDown:

        ret = "PageDown";
        break;
    case Keyboard::End:

        ret = "End";
        break;
    case Keyboard::Home:

        ret = "Home";
        break;
    case Keyboard::Insert:

        ret = "Insert";
        break;
    case Keyboard::Delete:

        ret = "Delete";
        break;
    case Keyboard::Add:

        ret = "Add";
        break;
    case Keyboard::Subtract:

        ret = "Subtract";
        break;
    case Keyboard::Multiply:

        ret = "Multiply";
        break;
    case Keyboard::Divide:

        ret = "Divide";
        break;
    case Keyboard::Left:

        ret = "Left";
        break;
    case Keyboard::Right:

        ret = "Right";
        break;
    case Keyboard::Up:

        ret = "Up";
        break;
    case Keyboard::Down:

        ret = "Down";
        break;
    case Keyboard::Numpad0:

        ret = "Numpad0";
        break;
    case Keyboard::Numpad1:

        ret = "Numpad1";
        break;
    case Keyboard::Numpad2:

        ret = "Numpad2";
        break;
    case Keyboard::Numpad3:

        ret = "Numpad3";
        break;
    case Keyboard::Numpad4:

        ret = "Numpad4";
        break;
    case Keyboard::Numpad5:

        ret = "Numpad5";
        break;
    case Keyboard::Numpad6:

        ret = "Numpad6";
        break;
    case Keyboard::Numpad7:

        ret = "Numpad7";
        break;
    case Keyboard::Numpad8:

        ret = "Numpad8";
        break;
    case Keyboard::Numpad9:

        ret = "Numpad9";
        break;
    case Keyboard::F1:

        ret = "F1";
        break;
    case Keyboard::F2:

        ret = "F2";
        break;
    case Keyboard::F3:

        ret = "F3";
        break;
    case Keyboard::F4:

        ret = "F4";
        break;
    case Keyboard::F5:

        ret = "F5";
        break;
    case Keyboard::F6:

        ret = "F6";
        break;
    case Keyboard::F7:

        ret = "F7";
        break;
    case Keyboard::F8:

        ret = "F8";
        break;
    case Keyboard::F9:

        ret = "F9";
        break;
    case Keyboard::F10:

        ret = "F10";
        break;
    case Keyboard::F11:

        ret = "F11";
        break;
    case Keyboard::F12:

        ret = "F12";
        break;
    case Keyboard::F13:

        ret = "F13";
        break;
    case Keyboard::F14:

        ret = "F14";
        break;
    case Keyboard::F15:

        ret = "F15";
        break;
    case Keyboard::Pause:

        ret = "Pause";
        break;
    case Keyboard::KeyCount:

        ret = "KeyCount";
        break;


    default:
        ret = "Unknown";
        break;
    }
    return ret;
}

Keyboard::Key Engine::String2Key(const string s) {
    if (s == "Left") {
        return Keyboard::Left;
    }
    if (s == "Right") {
        return Keyboard::Right;
    }
    if (s == "Up") {
        return Keyboard::Up;
    }
    if (s == "Down") {
        return Keyboard::Down;
    }
    if (s == "Space") {
        return Keyboard::Space;
    }
    if (s == "A") {
        return Keyboard::A;
    }
    if (s == "B") {
        return Keyboard::B;
    }
    if (s == "C") {
        return Keyboard::C;
    }
    if (s == "D") {
        return Keyboard::D;
    }
    if (s == "E") {
        return Keyboard::E;
    }
    if (s == "F") {
        return Keyboard::F;
    }
    if (s == "G") {
        return Keyboard::G;
    }
    if (s == "H") {
        return Keyboard::H;
    }
    if (s == "I") {
        return Keyboard::I;
    }
    if (s == "J") {
        return Keyboard::J;
    }
    if (s == "K") {
        return Keyboard::K;
    }
    if (s == "L") {
        return Keyboard::L;
    }
    if (s == "M") {
        return Keyboard::M;
    }
    if (s == "N") {
        return Keyboard::N;
    }
    if (s == "O") {
        return Keyboard::O;
    }
    if (s == "P") {
        return Keyboard::P;
    }
    if (s == "Q") {
        return Keyboard::Q;
    }
    if (s == "R") {
        return Keyboard::R;
    }
    if (s == "S") {
        return Keyboard::S;
    }
    if (s == "T") {
        return Keyboard::T;
    }
    if (s == "U") {
        return Keyboard::U;
    }
    if (s == "V") {
        return Keyboard::V;
    }
    if (s == "W") {
        return Keyboard::W;
    }
    if (s == "X") {
        return Keyboard::X;
    }
    if (s == "Y") {
        return Keyboard::Y;
    }
    if (s == "Z") {
        return Keyboard::Z;
    }
    if (s == "Num0") {
        return Keyboard::Num0;
    }
    if (s == "Num1") {
        return Keyboard::Num1;
    }
    if (s == "Num2") {
        return Keyboard::Num2;
    }
    if (s == "Num3") {
        return Keyboard::Num3;
    }
    if (s == "Num4") {
        return Keyboard::Num4;
    }
    if (s == "Num5") {
        return Keyboard::Num5;
    }
    if (s == "Num6") {
        return Keyboard::Num6;
    }
    if (s == "Num7") {
        return Keyboard::Num7;
    }
    if (s == "Num8") {
        return Keyboard::Num8;
    }
    if (s == "Num9") {
        return Keyboard::Num9;
    }
    if (s == "Escape") {
        return Keyboard::Escape;
    }
    if (s == "LControl") {
        return Keyboard::LControl;
    }
    if (s == "LShift") {
        return Keyboard::LShift;
    }
    if (s == "LAlt") {
        return Keyboard::LAlt;
    }
    if (s == "LSystem") {
        return Keyboard::LSystem;
    }
    if (s == "RControl") {
        return Keyboard::RControl;
    }
    if (s == "RShift") {
        return Keyboard::RShift;
    }
    if (s == "RAlt") {
        return Keyboard::RAlt;
    }
    if (s == "RSystem") {
        return Keyboard::RSystem;
    }
    if (s == "Menu") {
        return Keyboard::Menu;
    }
    if (s == "LBracket") {
        return Keyboard::LBracket;
    }
    if (s == "RBracket") {
        return Keyboard::RBracket;
    }
    if (s == "SemiColon") {
        return Keyboard::SemiColon;
    }
    if (s == "Comma") {
        return Keyboard::Comma;
    }
    if (s == "Period") {
        return Keyboard::Period;
    }
    if (s == "Quote") {
        return Keyboard::Quote;
    }
    if (s == "Slash") {
        return Keyboard::Slash;
    }
    if (s == "BackSlash") {
        return Keyboard::BackSlash;
    }
    if (s == "Tilde") {
        return Keyboard::Tilde;
    }
    if (s == "Equal") {
        return Keyboard::Equal;
    }
    if (s == "Dash") {
        return Keyboard::Dash;
    }
    if (s == "Return") {
        return Keyboard::Return;
    }
    if (s == "BackSpace") {
        return Keyboard::BackSpace;
    }
    if (s == "Tab") {
        return Keyboard::Tab;
    }
    if (s == "PageUp") {
        return Keyboard::PageUp;
    }
    if (s == "PageDown") {
        return Keyboard::PageDown;
    }
    if (s == "End") {
        return Keyboard::End;
    }
    if (s == "Home") {
        return Keyboard::Home;
    }
    if (s == "Insert") {
        return Keyboard::Insert;
    }
    if (s == "Delete") {
        return Keyboard::Delete;
    }
    if (s == "Add") {
        return Keyboard::Add;
    }
    if (s == "Subtract") {
        return Keyboard::Subtract;
    }
    if (s == "Multiply") {
        return Keyboard::Multiply;
    }
    if (s == "Divide") {
        return Keyboard::Divide;
    }
    if (s == "Numpad0") {
        return Keyboard::Numpad0;
    }
    if (s == "Numpad1") {
        return Keyboard::Numpad1;
    }
    if (s == "Numpad2") {
        return Keyboard::Numpad2;
    }
    if (s == "Numpad3") {
        return Keyboard::Numpad3;
    }
    if (s == "Numpad4") {
        return Keyboard::Numpad4;
    }
    if (s == "Numpad5") {
        return Keyboard::Numpad5;
    }
    if (s == "Numpad6") {
        return Keyboard::Numpad6;
    }
    if (s == "Numpad7") {
        return Keyboard::Numpad7;
    }
    if (s == "Numpad8") {
        return Keyboard::Numpad8;
    }
    if (s == "Numpad9") {
        return Keyboard::Numpad9;
    }
    if (s == "F1") {
        return Keyboard::F1;
    }
    if (s == "F2") {
        return Keyboard::F2;
    }
    if (s == "F3") {
        return Keyboard::F3;
    }
    if (s == "F4") {
        return Keyboard::F4;
    }
    if (s == "F5") {
        return Keyboard::F5;
    }
    if (s == "F6") {
        return Keyboard::F6;
    }
    if (s == "F7") {
        return Keyboard::F7;
    }
    if (s == "F8") {
        return Keyboard::F8;
    }
    if (s == "F9") {
        return Keyboard::F9;
    }
    if (s == "F10") {
        return Keyboard::F10;
    }
    if (s == "F11") {
        return Keyboard::F11;
    }
    if (s == "F12") {
        return Keyboard::F12;
    }
    if (s == "F13") {
        return Keyboard::F13;
    }
    if (s == "F14") {
        return Keyboard::F14;
    }
    if (s == "F15") {
        return Keyboard::F15;
    }
    if (s == "Pause") {
        return Keyboard::Pause;
    }
    if (s == "KeyCount") {
        return Keyboard::KeyCount;
    }
    // default
    return Keyboard::Unknown;
}