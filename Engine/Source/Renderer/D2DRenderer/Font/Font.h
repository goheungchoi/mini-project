#pragma once
#include "Core/Types/FontType.h"
#include "Renderer/D2DRenderer/Common.h"
#include <dwrite_3.h>

class Font
{
public:
  Font();
  ~Font();

public:
  IDWriteFactory5* GetIDWriteFactory() { return _pDWriteFactory; }
  void LoadFontFile(std::wstring fontPath, std::wstring fontName);
  IDWriteFontCollection1* GetFontCollection(const std::wstring& fontName);

private:
  void Init();
  void UnInit();

private:
  IDWriteFactory5* _pDWriteFactory = nullptr;
  IDWriteFontSetBuilder1* _pfontSetBuilder;

  std::unordered_map<std::wstring, IDWriteFontCollection1*>
      _fontCollections; // 폰트 이름으로 컬렉션 매핑
};
