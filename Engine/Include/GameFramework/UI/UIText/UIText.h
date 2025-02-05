#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"

class UIText : public UIElement
{
public:
  UIText(class World* world);
  virtual ~UIText();

public:
  void Render() override;

  void SetText(const wchar_t* format);
  void SetFont(const std::wstring& fontName);
  void SetColor(Color color);

private:
  class Text* _text{};
};
