#pragma once
#include "../Core/Common.h"

class WinApp
{
public:
  WinApp(const wchar_t* className, HINSTANCE hInstance);
  ~WinApp() = default;

public:
  HWND GetHWND() { return _hwnd; }
  void App_CreateWindow(int width, int height);

protected:
  HINSTANCE		_hInstance{};
  HWND			_hwnd{};
};
