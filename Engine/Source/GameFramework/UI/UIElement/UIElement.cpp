#include "GameFramework/UI/UIElement/UIElement.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void UIElement::BeginLevel()
{
  _status = EStatus_Active;
}

void UIElement::Update(float dt) {}

void UIElement::Render() {}

// pos = center position
void UIElement::SetPosition(Vector2 pos)
{

  _position.x = pos.x - (_size.x / 2);
  _position.y = pos.y - (_size.y / 2);

  if (_ownerPanel)
  {
    _position += _ownerPanel->GetPosition(); 
  }
}
