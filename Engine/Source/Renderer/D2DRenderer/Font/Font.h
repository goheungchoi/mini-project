#pragma once
#include "REnderer/D2DRenderer/Common.h"

class Font
{
public:
  Font();
  ~Font();

public:
  IDWriteTextFormat* FindFont(const std::wstring& fontName);

private:
  void Init();
  void UnInit();

  void CreateIDWriteFactory();

  void CreateTextFormat(
      const std::wstring& fontName, float size,
      UINT fontWeight = DWRITE_FONT_WEIGHT_REGULAR,
      UINT textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER,
      UINT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


private:
  IDWriteFactory* pDWriteFactory = nullptr;
  std::unordered_map<std::wstring, IDWriteTextFormat*> _Fonts;


  // ��ǻ�Ϳ� ��ġ�Ǿ��ִ� �۲� ���� ������ �ִ� �۲� ������ ���� �ڵ� �߰�

};

class Text
{
public:
  Text(const wchar_t* format, Vector4 rect,
       const std::wstring& fontName = L"Agency FB",
       Color color = Color(1.0f, 0.0f, 1.0f, 1.0f))
      : _format(format), _rect(rect), _fontName(fontName), _color(color)

  {
  }

  ~Text() = default;

public:
  std::wstring _format;
  Vector4 _rect;
  std::wstring _fontName;
  Color _color;

};