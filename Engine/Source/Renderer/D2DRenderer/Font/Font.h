#pragma once
#include "REnderer/D2DRenderer/Common.h"

enum class FontWeight
{
  THIN = 100,
  EXTRA_LIGHT = 200,
  LIGHT = 300,
  NORMAL = 400,
  MEDIUM = 500,
  SEMI_BOLD = 600,
  BOLD = 700,
  EXTRA_BOLD = 800,
  HEAVY = 900
};

enum class FontStyle
{
  NORMAL,
  ITALIC
};

enum class FontStretch
{
  UNDEFINED = 0,
  ULTRA_CONDENSED = 1,
  EXTRA_CONDENSED = 2,
  CONDENSED = 3,
  SEMI_CONDENSED = 4,
  NORMAL = 5,
  SEMI_EXPANDED = 6,
  EXPANDED = 7,
  EXTRA_EXPANDED = 8,
  ULTRA_EXPANDED = 9
};

enum class TextAlignment
{
  LEADING,
  TRAILING,
  CENTER,
  JUSTIFIED
};

enum class ParagraphAlignment
{
  NEAr,
  FAr,
  CENTEr
};

struct TextFormatInfo
{
  std::wstring _fontName;
  float _fontSize = 30.0f;
  FontWeight _fontWeight = FontWeight::NORMAL;
  FontStyle _fontStyle = FontStyle::NORMAL;
  FontStretch _fontStretch = FontStretch::NORMAL;
  TextAlignment _textAlignment = TextAlignment::CENTER;
  ParagraphAlignment _paragraphAlignment = ParagraphAlignment::CENTEr;
};

class Font
{
public:
  Font();
  ~Font();

public:
  IDWriteFactory* GetIDWriteFactory() { return pDWriteFactory; }
  
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
