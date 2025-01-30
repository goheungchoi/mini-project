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

  CreateTextFormat(L"Agency FB", 100.0f);
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
      static_cast<DWRITE_FONT_WEIGHT>(fontWeight), DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL,
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



TextManager* TextManager::m_pInstance = nullptr;

TextManager* TextManager::GetInstance()
{
  if (m_pInstance == nullptr)
  {
    m_pInstance = new TextManager();
  }

  return m_pInstance;
}

Text* TextManager::GetText(const wchar_t* format)
{
  // 기존 리스트에서 동일한 format을 가진 Text 객체 찾기
  for (auto text : _textList)
  {
    if (text->_format == format)
    {
      return text;
    }
  }

  // 동일한 format이 없으면 새로운 Text 객체 생성
  Text* newText = new Text(format);
  _textList.push_back(newText);
  return newText;


  // text의 format이 수정되면 어떻게 할 것 인가?
}


void TextManager::Destory()
{
  if (!_textList.empty())
  {
    for (auto txt : _textList)
    {
      SAFE_RELEASE(txt);
    }
  }

  if (m_pInstance)
  {
    delete m_pInstance;
    m_pInstance = nullptr;
  }
}
