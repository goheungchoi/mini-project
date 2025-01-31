#include "GameFramework/UI/UIElement/UIElement.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void UIElement::BeginLevel()
{
  _status = EStatus_Active;
}

void UIElement::Update(float dt) {}

void UIElement::Render() {}

void UIElement::SetPosition(Vector2 pos)
{
  _position.x = pos.x;
  _position.y = pos.y;

  if (_ownerPanel)
  {
    _position.x += _ownerPanel->GetPosition().x;
    _position.y += _ownerPanel->GetPosition().y;
  }
}
