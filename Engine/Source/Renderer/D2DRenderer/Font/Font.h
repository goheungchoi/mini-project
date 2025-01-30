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


  // 컴퓨터에 설치되어있는 글꼴 말고 폴더에 있는 글꼴 가져다 쓰는 코드 추가

};

class Text
{
public:
  Text(const wchar_t* format) : _format(format) {}
  Text(const wchar_t* format, Vector4 rect,
       const std::wstring& fontName = L"Agency FB",
       Color color = Color(1.0f, 0.0f, 1.0f, 1.0f))
      : _format(format), _rect(rect), _fontName(fontName), _color(color) {}

  ~Text() = default;

public:
  std::wstring _format = L"NULL";
  Vector4 _rect{0, 0, 200, 200};
  std::wstring _fontName = L"Agency FB";
  Color _color = Color(0.0f, 0.0f, 0.0f, 1.0f);

};

class TextManager
{
private:
  TextManager() = default;
  ~TextManager() = default;

  TextManager(const TextManager&) = delete;
  TextManager& operator=(const TextManager&) = delete;
  TextManager(TextManager&&) noexcept = delete;
  TextManager& operator=(TextManager&&) noexcept = delete;

  static TextManager* m_pInstance;

public:
  static TextManager* GetInstance();
  Text* GetText(const wchar_t* format);
  void Destory();

public:
  std::list<Text*> _textList;
};

#define _TextManager (*TextManager::GetInstance())
