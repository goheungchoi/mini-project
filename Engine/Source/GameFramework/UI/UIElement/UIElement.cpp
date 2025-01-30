#include "GameFramework/UI/UIElement/UIElement.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void UIElement::BeginLevel()
{
  _status = EStatus_Active;
}

void UIElement::Update(float dt) {}

void UIElement::Render() {}
