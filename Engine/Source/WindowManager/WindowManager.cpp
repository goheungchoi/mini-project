#include "WindowManager.h"
#include "../WinApp/WinApp.h"

HWND WindowManager::CreateWinApp(const wchar_t* className, HINSTANCE hInstance)
{
  WinApp* newWinApp = new WinApp(className, hInstance);
  return HWND();
}
