#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <mutex>
#include <future>
#include "lib_ecm.h"
#include "SFML/Audio.hpp"
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
	shared_ptr<Entity> makeEntity(string enName);

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
	static void PlayAudio(Sound);
	static RenderWindow& GetWindow();
	static Vector2u GetWindowSize();
	static void ChangeWindowMode(const string);
	static void SetVsync(const bool);

	// Get and set sector values
	static Borders GetCentreSectorBorders();
	static Vector2f GetCentreSectorSize();
	static void SetCentreSectorSize(const Vector2f);

	// Controls
	static map<string, Keyboard::Key> GetControls();
	static void SetControl(const string controlName, const Keyboard::Key);
	static string Key2String(const Keyboard::Key);
	static Keyboard::Key String2Key(const string s);
	static void ObserveControlChange(const string keyName); // mark to start detecting key to change a control from the string to
	static bool isObservingControlChange();

	// Saving
	static bool SaveScore(const int level, const string); // returns false if it isn't a new high score, else return true
	static string GetScore(const int level);
	static void SaveControls();
	static void UpdateSavedControls();
	static void SaveWinMode(const bool);
	static bool GetWinMode();

	// Database
	static void SetDatabaseLocation(const string s) { _databaseLocation = s; }

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
	static bool _isObservingControlChange;
	static string _observingControlName; // empty string is no, a string with contents marks the control to change

	// Location of the database
	static string _databaseLocation;
};