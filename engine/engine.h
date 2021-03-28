#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <mutex>
#include <future>
#include "lib_ecm.h"

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
	shared_ptr<Entity> makeEntity();

protected:
	void setLoaded(bool);

private:
	EntityManager ents;
	mutable bool _loaded;
	mutable future<void> _loaded_future;
	mutable mutex _loaded_mtx;
};

class Engine {
public:
	Engine() = delete;

	static void Start(unsigned int winWidth, unsigned int winHeight,
		const string& gameName, Scene* scene);
	static void ChangeScene(Scene*);

	static RenderWindow& GetWindow();
	static Vector2u GetWindowSize();
	static void setVsync(bool);

private:
	static Scene* _activeScene;
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