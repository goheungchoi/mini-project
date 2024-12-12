#include "WinApp.h"

WinApp::WinApp(const wchar_t* className, HINSTANCE hInstance)
{
  _className = className;
  _hInstance = hInstance;
}
