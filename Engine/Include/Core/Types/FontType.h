#pragma once

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

  TextFormatInfo(){}
  TextFormatInfo(std::wstring fontName) : _fontName(fontName) {}
};