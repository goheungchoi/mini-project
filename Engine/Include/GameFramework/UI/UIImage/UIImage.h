#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"

class UIImage : public UIElement
{
public:
  UIImage(class World* world);
  virtual ~UIImage();

public:
  void SetSprite(LPCSTR path, Vector2 pos = {0.0f, 0.0f});
  void SetSprite(class Sprite* sprite) { sprite = _sprite; }

  void SetOpacity(float opacity) { opacity = _opacity; }
  void SetPosition(Vector2 pos) override;

private:
  class Sprite* _sprite{};
};
