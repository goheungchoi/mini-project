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
  float GetOpacity() { return _opacity; }

  void SetLateRender(bool flag) { bLateRender = flag; }

  virtual void SetCenterPos(Vector2 pos) override;

  void SetScale(Vector2 scale);

  void FadeIn(float threshold);
  void FadeOut(float threshold);
  void Transition(float dt);

  virtual void SetMasking(Color _col);

  Vector2 GetTextureSize();

  bool _transitionFlag = false;

protected:
  class Sprite* _sprite{};
  bool bLateRender = false;
  float _transitionElaspedTimer = 0.f;
  float _transitionThreshold = 1.f;
  float _transitionOpacity = 0.f;
  float _transitionStartOpacity = 0.f;
  float _transitionEndOpacity = 0.f;

  bool _maskFlag = false;
  Vector2 _scale = {1.0f, 1.0f};

};
