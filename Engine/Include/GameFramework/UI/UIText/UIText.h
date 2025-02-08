#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"

class UIText : public UIElement
{
public:
  UIText(class World* world);
  virtual ~UIText();

public:
  void Render() override;

  void SetText(const wstring format);
  void SetFont(const std::wstring& fontName);
  void SetFontSize(const float fontSize);
  void SetColor(Color color);
  void SetOpacity(float opacity);
  void SetFontWeight(FontWeight fontWeight);
  void SetTextAlignment(TextAlignment textAlignment);
  void SetParagraphAlignment(ParagraphAlignment paragraphAlignment);

#ifdef _DEBUG
  void SetDebugDraw(bool debugFlag);
#endif

private:
  TextFormatInfo _textFormatInfo;
  std::wstring _format;
  Vector4 _rect;
  Color _color;
  float _opacity;

#ifdef _DEBUG
  bool bDebugDrawFlag = false;
#endif
};
