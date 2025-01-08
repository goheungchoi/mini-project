#pragma once
#include "Core/Common.h"

// MyRegisterClass() 에 들어감
class WindowClass
{
  WNDCLASS _wndClass;
};

// CreateWindow() 에 들어감
class WindowStyle
{

};

class WinApp
{
public:
  WinApp(const wchar_t* className, HINSTANCE hInstance);
  ~WinApp() = default;

public:
  HWND GetHWND() { return _hwnd; }
  void App_CreateWindow(int width = 1920, int height = 1080);
  static LRESULT CALLBACK HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                 LPARAM lParam);


protected:
  HINSTANCE		_hInstance{};
  HWND			_hwnd{};
  const wchar_t* _className{};
};
