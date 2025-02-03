#include "Font.h"
#include "Renderer/D2DRenderer/D2DRenderer.h"
#include "Renderer/DX11/Internal/Device.h"

/// <summary>
////////////////////// Font Engine //////////////////////////
/// </summary>

Font::Font()
{
  Init();
}

Font::~Font()
{
  UnInit();
}

void Font::Init()
{
  CreateIDWriteFactory();

  CreateTextFormat(L"Agency FB", 30.0f);
  CreateTextFormat(L"궁서", 32.0f);
}

void Font::UnInit()
{
  for (auto& pair : _Fonts)
  {
    if (pair.second)
    {
      Com::SAFE_RELEASE(pair.second);
    }
  }

  _Fonts.clear();

  // IDWriteFactory 파괴
  Com::SAFE_RELEASE(pDWriteFactory);
}

void Font::CreateIDWriteFactory()
{
  HR_T(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                           reinterpret_cast<IUnknown**>(&pDWriteFactory)));
}

void Font::CreateTextFormat(const std::wstring& fontName, float size,
                            UINT fontWeight, UINT textAlignment,
                            UINT paragraphAlignment)
{
  IDWriteTextFormat* pTextFormat;

  // const wchar_t* formatName = L"맑은 고딕";

  HR_T(pDWriteFactory->CreateTextFormat(
      fontName.c_str(), // 글꼴 이름
      NULL,             // 글꼴 컬렉션 (NULL은 시스템 기본 사용)
      static_cast<DWRITE_FONT_WEIGHT>(fontWeight),
      static_cast<DWRITE_FONT_STYLE> (FontStyle::NORMAL),
      static_cast<DWRITE_FONT_STRETCH>(FontStretch::NORMAL),
      size,     // 글꼴 크기
      L"ko-KR", // 로케일
      &pTextFormat));

  // 텍스트 정렬
  pTextFormat->SetTextAlignment(
      static_cast<DWRITE_TEXT_ALIGNMENT>(textAlignment));
  pTextFormat->SetParagraphAlignment(
      static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(paragraphAlignment));

  _Fonts.insert({fontName, pTextFormat});
}


IDWriteTextFormat* Font::FindFont(const std::wstring& fontName)
{
  auto iter = _Fonts.find(fontName);

  if (iter != _Fonts.end())
  {
    return iter->second;
  }
  else
  {
      // 폰트 생성
  }

  return nullptr;
}