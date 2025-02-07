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

void Font::LoadFontFile(std::wstring fontPath, std::wstring fontName)
{
  IDWriteFontFile* pFontFile{nullptr};
  IDWriteFontSet* pFontSet{nullptr};
  IDWriteFontCollection1* pFontCollection{nullptr};
  IDWriteFontFamily* pFontFamily{nullptr};
  IDWriteLocalizedStrings* pFontFamilyNames{nullptr};

  // IDWriteFontFile 객체 생성
  HR_T(_pDWriteFactory->CreateFontFileReference(fontPath.c_str(), nullptr,
                                                &pFontFile));

  // 폰트 파일을 빌더에 추가

  HR_T(_pfontSetBuilder->AddFontFile(pFontFile));
  //  }

  // 폰트 세트를 생성합니다. 이 세트는 추가된 폰트 파일들을 포함합니다.
  HR_T(_pfontSetBuilder->CreateFontSet(&pFontSet));

  // 폰트 세트에서 폰트 컬렉션 생성
  HR_T(_pDWriteFactory->CreateFontCollectionFromFontSet(pFontSet,
                                                        &pFontCollection));

  // 생성된 컬렉션을 맵에 저장
  _fontCollections[fontName] = pFontCollection;

  // 폰트 패밀리 이름 검증 (옵션)
  UINT32 familyCount = pFontCollection->GetFontFamilyCount();
  // 폰트 패밀리 이름 검증 강화
  if (familyCount > 0)
  {
    IDWriteFontFamily* pFamily;
    pFontCollection->GetFontFamily(0, &pFamily);

    IDWriteLocalizedStrings* pNames;
    pFamily->GetFamilyNames(&pNames);

    wchar_t actualName[128];
    pNames->GetString(0, actualName, 128);

    if (fontName != actualName)
    {
      std::wstring errorMsg = L"[경고] 폰트 파일 내 패밀리 이름 불일치:\n";
      errorMsg += L"등록 이름: " + fontName + L"\n";
      errorMsg += L"실제 이름: " + std::wstring(actualName);
      MessageBoxW(nullptr, errorMsg.c_str(), L"Font Warning", MB_ICONWARNING);
    }
  }
}

IDWriteFontCollection1* Font::GetFontCollection(const std::wstring& fontName)
{
  auto it = _fontCollections.find(fontName);
  return (it != _fontCollections.end()) ? it->second : nullptr;
}

void Font::Init()
{
  HR_T(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                           __uuidof(IDWriteFactory5),
                           reinterpret_cast<IUnknown**>(&_pDWriteFactory)));

  HR_T(_pDWriteFactory->CreateFontSetBuilder(&_pfontSetBuilder));
}

void Font::UnInit()
{
  // IDWriteFactory 파괴
  Com::SAFE_RELEASE(_pDWriteFactory);
}
