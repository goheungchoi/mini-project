#pragma once
#include "Core/Common.h"
#include "WindowManager/WindowManager.h"

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
  friend class WindowManager;

public:
  WinApp(const wchar_t* className, HINSTANCE hInstance);
  ~WinApp() = default;

public:
  HWND GetHWND() const { return _hwnd; }
  void App_CreateWindow(int width = 1920, int height = 1080);
  static LRESULT CALLBACK HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                 LPARAM lParam);


protected:
  HINSTANCE		_hInstance{};
  HWND			_hwnd{};
  const wchar_t* _className{};

private:
  std::function<bool()> _onActivated;
  std::function<bool()> _onDeactivated;
  std::function<bool()> _onSuspending;
  std::function<bool()> _onResuming;
  std::function<bool()> _onWindowResized;

};
