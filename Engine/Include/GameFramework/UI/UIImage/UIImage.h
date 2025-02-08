#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"

class UIImage : public UIElement
{
public:
  UIImage(class World* world);
  virtual ~UIImage();

public:
  void Update(float dt) override;
  void Render() override;
  void RenderTransition();

  void SetSprite(LPCSTR path, Vector2 pos = {0.0f, 0.0f});
  void SetSprite(class Sprite* sprite) { _sprite = sprite; }

  void SetOpacity(float opacity);
 
  virtual void SetCenterPos(Vector2 pos) override;

  void SetScale(Vector2 scale);

  void FadeIn(float threshold);
  void FadeOut(float threshold);
  void Transition(float dt);

private:
  class Sprite* _sprite{};

  float _transitionElaspedTimer = 0.f;
  float _transitionThreshold = 1.f;
  float _transitionOpacity = 0.f;
  float _transitionStartOpacity = 0.f;
  float _transitionEndOpacity = 0.f;
  bool _transitionFlag = false;

  Vector2 _scale = {1.0f, 1.0f};

};
