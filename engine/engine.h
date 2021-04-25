#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <mutex>
#include <future>
#include "lib_ecm.h"

// Scene switch timer
static float scnSwitchTimer = 1;

// Parent Scene
class Scene {
public:
	Scene() = default;
	virtual ~Scene();

	virtual void Update(const double dt);
	virtual void Render();

	virtual void Load() = 0;
	virtual void LoadAsync();
	virtual void UnLoad();
	bool isLoaded() const;

	EntityManager getEnts();
	shared_ptr<Entity> makeEntity(int orderNum);

	string getSceneName() const;
	void setSceneName(const string);

	virtual void DefaultSetup() {}

protected:
	void setLoaded(bool);
	EntityManager ents;

private:
	mutable bool _loaded;
	mutable future<void> _loaded_future;
	mutable mutex _loaded_mtx;
	string _sceneName;
};

// Borders of a rectangle
struct Borders {
	float top;
	float bottom;
	float left;
	float right;
};

class Engine {
public:
	Engine() = delete;

	static void Start(unsigned int winWidth, unsigned int winHeight,
		const string& gameName, Scene* scene, const bool isFullscreen);
	static void ChangeScene(Scene*);
	static void PauseScene(Scene*);
	static void UnloadPreviousScene();

	static RenderWindow& GetWindow();
	static Vector2u GetWindowSize();
	static void ChangeWindowMode(const string);
	static void SetVsync(const bool);

	// Get and set sector values
	static Borders GetCentreSectorBorders();
	static Vector2f GetCentreSectorSize();
	static void SetCentreSectorSize(const Vector2f);

	//Controls
	static map<string, Keyboard::Key> GetControls();
	static void SetControl(const string controlName, const Keyboard::Key);
	static string Key2String(const Keyboard::Key);

private:
	static Scene* _activeScene;
	static Scene* _previousScene;
	static string _gameName;
	static void Update();
	static void Render(RenderWindow& window);

	// Flag to change the window mode - '' means no change, name indicates the type of change
	static string _changingMode;

	// Centre sector of the window
	static Vector2f _centreSector;

	// Controls
	static map<string, Keyboard::Key> _controls;
};

// Timing
namespace timing {
	// Return time since Epoc
	long long now();
	// Return time since last() was last called.
	long long last();
}