#include "cmp_text.h"
#include <system_renderer.h>
#include <resources_system.h>

void TextComponent::Update(double dt) {}

void TextComponent::Render() { 
    Renderer::Queue(&_text); 
}

TextComponent::TextComponent(Entity* const p, const string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<Font>("Roboto-Bold.ttf");
  _text.setFont(*_font);
}

void TextComponent::SetText(const string& str) {
  _string = str;
  _text.setString(_string);
}
