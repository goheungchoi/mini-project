#pragma once
#include "../Core/Common.h"

class WinApp
{
public:
  WinApp(const wchar_t* className, HINSTANCE hInstance);
  ~WinApp() = default;

public:

	
protected:
  HINSTANCE		_hInstance{};
  HWND			_hwnd{};

  // Test
  const wchar_t* _className;
};
