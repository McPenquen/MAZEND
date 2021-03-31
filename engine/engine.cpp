#include "engine.h"
#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include "system_renderer.h"
#include <maths.h>

// Scene
void Scene::Update(const double dt) { 
	ents.Update(dt); 
}

void Scene::Render() { 
	ents.Render(); 
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
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() {
	_loaded_future = async(&Scene::Load, this);
}

Scene::~Scene() {
	UnLoad();
}

shared_ptr<Entity> Scene::makeEntity() {
	auto en = make_shared<Entity>(this);
	ents.list.push_back(en);
	return move(en);
}

// Engine
Scene* Engine::_activeScene = nullptr;
string Engine::_gameName;

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
	octagon.setPosition(sf::Vcast<float>(Engine::GetWindowSize()) * .5f);
	octagon.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	static Text t;
	t.setString("Loading Level");
	loadingFont.loadFromFile("res/fonts/Roboto-Bold.ttf");
	t.setFont(loadingFont);
	t.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	Vector2f winPos = sf::Vcast<float>(Engine::GetWindowSize());
	t.setPosition(winPos * Vector2f(0.4f, 0.3f));
	Renderer::Queue(&t);
	Renderer::Queue(&octagon);
}

// - Engine
void Engine::Update() {
	static Clock clock;
	float dt = clock.restart().asSeconds();

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

void Engine::Start(unsigned int width, unsigned int height, const string& gameName, Scene* scene) {
	RenderWindow window(VideoMode(width, height), gameName);
	_gameName = gameName;
	_window = &window;
	Renderer::Initialise(window);
	ChangeScene(scene);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
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
	window.close();
}

void Engine::setVsync(bool bo) {
	_window->setVerticalSyncEnabled(bo);
}

void Engine::ChangeScene(Scene* s) {
	cout << "Eng: changing scene: " << s << endl;
	auto oldS = _activeScene;
	_activeScene = s;

	if (oldS != nullptr) {
		oldS->UnLoad();
	}

	if (!s->isLoaded()) {
		cout << "Eng: Entering Loading Screen\n";
		loadingTime = 0;
		_activeScene->LoadAsync();
		loading = true;
	}
}

Vector2u Engine::GetWindowSize() {
	return _window->getSize();
}

RenderWindow& Engine::GetWindow() {
	return *_window;
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
