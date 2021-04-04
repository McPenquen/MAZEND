#include "resources_system.h"

namespace Resources {
    template <> shared_ptr<Font> load(const string& name) {
        auto f = make_shared<Font>();
        if (!f->loadFromFile("res/fonts/" + name)) {
            throw("not found: " + name);
        }
        return f;
    };

    template <> shared_ptr<Texture> load(const string& name) {
        auto tex = make_shared<Texture>();
        if (!tex->loadFromFile("res/img/" + name)) {
            throw("not found: " + name);
        };
        return tex;
    };

#ifdef SOUND
    template <> shared_ptr<SoundBuffer> load(const string& name) {
        auto buf = make_shared<SoundBuffer>();
        if (!buf->loadFromFile("res/sound/" + name)) {
            throw("not found: " + name);
        };
        return buf;
    };

    template <> shared_ptr<Music> load(const string& name) {
        auto music = make_shared<Music>();
        if (!music->openFromFile("res/sound/" + name)) {
            throw("not found: " + name);
        };
        return music;
    };
#endif

}