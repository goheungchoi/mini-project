#include "GameFramework/UI/UIText/UIText.h"
#include "Renderer/D2DRenderer/Font/Font.h"

UIText::UIText(World* world) : UIElement(world) {}

UIText::~UIText() {}

void UIText::Render()
{
  _world->_renderer->DrawTexts(_text->_format.c_str(), _text->_rect,
                               _text->_color, _text->_fontName);
}

void UIText::SetText(const wchar_t* format)
{
  _text->_format = format;
}

void UIText::SetFont(const std::wstring& fontName)
{
  _text->_fontName = fontName;
}

void UIText::SetColor(Color color)
{
  _text->_color = color;
}
