#include "engine.h"
#include <chrono>
#include <future>
#include <iostream>
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
	ents.floor1_list.clear();
	ents.floor2_list.clear();
	ents.floor3_list.clear();
	ents.floor4_list.clear();
	ents.players.clear();
	ents.enemies.clear();
	setLoaded(false);
}

void Scene::LoadAsync() {
	_loaded_future = async(&Scene::Load, this);
}

Scene::~Scene() {
	UnLoad();
}

shared_ptr<Entity> Scene::makeEntity(int orderNum) {
	auto en = make_shared<Entity>(orderNum);
	// number 5 means the player list
	if (orderNum == 5) {
		ents.players.push_back(en);
	}
	// number 6 means an enemy list
	else if (orderNum == 6) {
		ents.enemies.push_back(en);
	}
	else if (orderNum == 1) {
		ents.floor1_list.push_back(en);
	}
	else if (orderNum == 2) {
		ents.floor2_list.push_back(en);
	}
	else if (orderNum == 3) {
		ents.floor3_list.push_back(en);
	}
	else {
		ents.floor4_list.push_back(en);
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
	cout << "Eng: Loading Screen\n";
	if (scene->isLoaded()) {
		cout << "Eng: Exiting Loading Screen\n";
		loading = false;
	}
	else {
		loadingProgress += 220.f * dt;
		loadingTime += dt;
	}
}

static Font loadingFont;

void LoadingRender() {
	cout << "Eng: Loading Screen Render\n";
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

float frametimes[256] = {};
uint8_t ftc = 0;

// - Engine
void Engine::Update() {
	static Clock clock;
	float dt = clock.restart().asSeconds();

	scnSwitchTimer += dt;

	{
    frametimes[++ftc] = dt;
    static string avg = _gameName + " FPS:";
    if (ftc % 60 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      _window->setTitle(avg + toStrDecPt(2, davg));
    }
  }

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
				Engine::Start(width, height, gameName, scene, 1);
			}
			else {
				window.close();
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
		cout << "Eng: changing scene: " << s << endl;
		_previousScene = _activeScene;
		_activeScene = s;

		if (_previousScene != nullptr) {
			_previousScene->UnLoad();
		}

		if (!s->isLoaded()) {
			cout << "Eng: Entering Loading Screen\n";
			loadingTime = 0;
			_activeScene->LoadAsync();
			loading = true;
		}
	}
}

void Engine::PauseScene(Scene* s) {
	if (scnSwitchTimer >= sceneSwithTime) {
		scnSwitchTimer = 0;
		cout << "Eng: changing scene: " << s << endl;
		_previousScene = _activeScene;
		_activeScene = s;

		if (!s->isLoaded()) {
			cout << "Eng: Entering Loading Screen\n";
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

// Timing
namespace timing {
	// Return time since epoch
	long long now() {
		return chrono::high_resolution_clock::now().time_since_epoch().count();
	}
	// Return the time since it was last called
	long long last() {
		auto cn = now();
		static auto then = now();
		auto dt = cn - then;
		then = cn;
		return dt;
	}
}
