#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"

class UIImage : public UIElement
{
public:
  UIImage(class World* world);
  virtual ~UIImage();

public:
  void SetSprite(LPCSTR path);
  void SetSprite(class Sprite* sprite) { sprite = _sprite; }

  void SetOpacity(float opacity) { opacity = _opacity; }

  void Render() override;

private:
  class Sprite* _sprite{};
};
