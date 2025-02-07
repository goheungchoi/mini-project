#include "GameFramework/UI/UIText/UIText.h"
#include "Renderer/D2DRenderer/Font/Font.h"

UIText::UIText(World* world) : UIElement(world)
{
  _textFormatInfo._fontName = L"GangwonEduAll";
  _textFormatInfo._fontSize = 30.0f;
  _textFormatInfo._fontWeight = FontWeight::BOLD;
  _textFormatInfo._fontStyle = FontStyle::NORMAL;
  _textFormatInfo._fontStretch = FontStretch::NORMAL;
  _textFormatInfo._textAlignment = TextAlignment::CENTERAlIGN;
  _textFormatInfo._paragraphAlignment = ParagraphAlignment::MIDALIGN;

  _color = Color(0, 0, 0, 1);
  _opacity = 1.0f;
  _size = Vector2(400, 200);
}

UIText::~UIText() {}

void UIText::Render()
{
  _rect = {_position.x, _position.y, _position.x + _size.x,
           _position.y + _size.y};

  _world->_renderer->DrawTexts(_format, _rect, _color, _opacity,
                               _textFormatInfo);
  #ifndef NDEBUG
  if (bDebugDrawFlag)
  {
    _world->_renderer->DrawRectangle(Color(1, 0, 0, 1), _rect);
  }
  #endif
}

void UIText::SetText(const wchar_t* format)
{
  _format = format;
}

void UIText::SetFont(const std::wstring& fontName)
{
  _textFormatInfo._fontName = fontName;
}

void UIText::SetColor(Color color)
{
  _color = color;
}

void UIText::SetOpacity(float opacity)
{
  _opacity = opacity;
}

void UIText::SetFontWeight(FontWeight fontWeight)
{
  _textFormatInfo._fontWeight = fontWeight;
}

void UIText::SetTextAlignment(TextAlignment textAlignment)
{
  _textFormatInfo._textAlignment = textAlignment;
}

void UIText::SetParagraphAlignment(ParagraphAlignment paragraphAlignment)
{
  _textFormatInfo._paragraphAlignment = paragraphAlignment;
}
#ifndef NDEBUG
void UIText::SetDebugDraw(bool debugFlag)
{
  bDebugDrawFlag = debugFlag;
}
#endif
