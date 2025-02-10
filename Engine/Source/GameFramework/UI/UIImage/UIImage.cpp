#include "GameFramework/UI/UIImage/UIImage.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"
#include "Resource2DManager/Resource2DManager.h"

UIImage::UIImage(World* world) : UIElement(world) {}

UIImage::~UIImage() {}

void UIImage::Update(float dt)
{
  UIElement::Update(dt);
  Transition(dt);
}

void UIImage::Render()
{
  if (_status == EStatus_Active)
  {
    //_sprite->Render();
    _sprite->bLateRender = bLateRender;
    _sprite->Render(_position, _scale, _opacity); // 인스턴스 속성 전달
  }
}

void UIImage::RenderTransition()
{
  _sprite->RenderTransition();
}

void UIImage::SetOpacity(float opacity)
{
    _opacity = opacity;
    _sprite->SetOpacity(opacity);
}

void UIImage::SetScale(::DirectX::SimpleMath::Vector2 scale)
{
  _scale = scale;
  _sprite->SetScale(scale);
}

void UIImage::FadeIn(float threshold)
{
  _transitionThreshold = threshold;
  _transitionStartOpacity = 0.f;
  _transitionEndOpacity = 1.f;
  _transitionFlag = true;
}

void UIImage::FadeOut(float threshold)
{
  _transitionThreshold = threshold;
  _transitionStartOpacity = 1.f;
  _transitionEndOpacity = 0.f;
  _transitionFlag = true;
}

void UIImage::Transition(float dt)
{
  if (_transitionFlag)
  {
    _transitionElaspedTimer += dt;
    if (_transitionElaspedTimer <= _transitionThreshold)
    {
      float transitionRatio = _transitionElaspedTimer / _transitionThreshold;
      _transitionOpacity = _transitionStartOpacity * (1 - transitionRatio) +
                           _transitionEndOpacity * transitionRatio;
    }
    else
    {
      _transitionOpacity = _transitionEndOpacity;
      _transitionFlag = false;
      _transitionElaspedTimer = 0.f;
    }
  SetOpacity(_transitionOpacity);
  }
}

void UIImage::SetMasking(Color _col)
{
    _sprite->SetMasking(_col);
}

void UIImage::SetSprite(LPCSTR path, Vector2 pos)
{
  _sprite = Resource2DManager::GetInstance()->GetSprite(path).get();
  _size = _sprite->GetTextureSize();
  //_sprite->SetPos({pos.x - (_size.x / 2), pos.y - (_size.y / 2)});

  _position = {pos.x - (_size.x / 2),
               pos.y - (_size.y / 2)}; // 위치 계산 후 저장
}

void UIImage::SetCenterPos(Vector2 pos)
{
  //UIElement::SetCenterPos(pos);
  _position = {pos.x - (_size.x / 2 * _scale.x),
               pos.y - (_size.y / 2 * _scale.y)};
  _sprite->SetPos(_position);
}
Vector2 UIImage::GetTextureSize()
{
  return _sprite->GetTextureSize();
}