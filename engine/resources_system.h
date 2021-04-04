#pragma once
#include <SFML/Audio.hpp>

#ifdef SOUND
#include <SFML/Audio/Sound.hpp>
#endif

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>

using namespace std;
using namespace sf;

namespace Resources{
	template <typename T> shared_ptr<T> load(const string& name) {
		throw("Resource loader unavailable for this type");
	}

	template <> shared_ptr<Font> load(const string& name);

	template <> shared_ptr<Texture> load(const string& name);

	template <typename T>
	static shared_ptr<T> get(const string& name) {
		static map<string, shared_ptr<T>> _things;
		auto search = _things.find(name);
		if (search != _things.end()) {
			return search->second;
		}
		else {
			_things[name] = load<T>(name);
			return _things[name];
		}
	};

#ifdef SOUND
	template <> shared_ptr<SoundBuffer> load(const string& name);

	template <> shared_ptr<Music> load(const string& name);
#endif
}

