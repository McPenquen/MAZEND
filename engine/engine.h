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


protected:
	void setLoaded(bool);
	EntityManager ents;

private:
	mutable bool _loaded;
	mutable future<void> _loaded_future;
	mutable mutex _loaded_mtx;
	string _sceneName;
};

class Engine {
public:
	Engine() = delete;

	static void Start(unsigned int winWidth, unsigned int winHeight,
		const string& gameName, Scene* scene);
	static void ChangeScene(Scene*);
	static void PauseScene(Scene*);
	static void UnloadPreviousScene();

	static RenderWindow& GetWindow();
	static Vector2u GetWindowSize();
	static void setVsync(bool);

private:
	static Scene* _activeScene;
	static Scene* _previousScene;
	static string _gameName;
	static void Update();
	static void Render(RenderWindow& window);
};

// Timing
namespace timing {
	// Return time since Epoc
	long long now();
	// Return time since last() was last called.
	long long last();
}