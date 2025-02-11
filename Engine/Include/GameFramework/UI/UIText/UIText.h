#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"

class UIText : public UIElement
{
public:
  UIText(class World* world);
  virtual ~UIText();

public:
  void Render() override;
  void Update(float dt) override;
  void SetText(const wchar_t* format);
  void SetFont(const std::wstring& fontName);
  void SetFontSize(const float fontSize);
  void SetColor(Color color);
  void SetOpacity(float opacity);
  void SetFontWeight(FontWeight fontWeight);
  void SetTextAlignment(TextAlignment textAlignment);
  void SetParagraphAlignment(ParagraphAlignment paragraphAlignment);
  void SetTypoAnim(bool istypo = false) { _bIsTypo = istypo; };
  void SetTypoInterval(float interval) { _typointerval = interval; };

  void Fade(float threshold, float start, float end);
  void Transition(float dt);




  void SetDebugDraw(bool debugFlag);


private:
  bool _bIsTypo = false;
  TextFormatInfo _textFormatInfo;
  wstring _format;
  wstring _typoFormat;
  Vector4 _rect;
  Color _color;
  float _opacity;

  float _typointerval = 0.1f;
  float _typoElapsedTimer= 0.f;
  int _typoIdx = 1;
  wchar_t _typoChar;

  bool bDebugDrawFlag = false;

  float _transitionElaspedTimer = 0.f;
  float _transitionThreshold = 1.f;
  float _transitionOpacity = 0.f;
  float _transitionStartOpacity = 0.f;
  float _transitionEndOpacity = 0.f;
  bool _transitionFlag = false;



};
