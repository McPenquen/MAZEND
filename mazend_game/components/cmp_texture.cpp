#include "cmp_texture.h"
#include <system_renderer.h>
#include <resources_system.h>

void TextComponent::Update(double dt) {
    _texture.setPosition(_parent->getPosition());
}

void TextComponent::Render() {
    Renderer::Queue(&_texture);
}

TextComponent::TextComponent(Entity* const p, const string& str)
    : Component(p), _string(str) {
    _texture.setS
    _font = Resources::get<Font>("Roboto-Bold.ttf");
    _texture.setFont(*_font);
}

void TextComponent::SetTexture(const string& str) {
    _string = str;
    _texture.setTexture(_string);
}
