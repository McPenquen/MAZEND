#pragma once
#include <SFML/Graphics/Text.hpp>
#include <lib_ecm.h>

class TextComponent : public Component {
public:
  TextComponent() = delete;
  ~TextComponent() override = default;

  explicit TextComponent(Entity* p, const string& str = "");

  void Update(double dt) override;
  void Render() override;

  void SetText(const string& str);

protected:
  shared_ptr<Font> _font;
  string _string;
  Text _text;
};
