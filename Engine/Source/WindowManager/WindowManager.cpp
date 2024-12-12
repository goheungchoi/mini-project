#include "WindowManager.h"
#include "../WinApp/WinApp.h"

WindowManager* WindowManager::m_pInstance = nullptr;

WindowManager* WindowManager::GetInstance()
{
  if (!m_pInstance)
  {
    m_pInstance = new WindowManager();
  }

  return m_pInstance;
}

WindowManager::WindowManager()
{
  _winApps.reserve(3); // º¤ÅÍ ÃÊ±âÈ­
}

WindowManager::~WindowManager()
{
    if (!_winApps.empty())
    {
      for (auto& winapp : _winApps)
      {
        delete winapp;
      }
    }
}

HWND WindowManager::CreateWinApp(int width, int height,
                                 const wchar_t* className, HINSTANCE hInstance)
{
  WinApp* newWinApp = new WinApp(className, hInstance);
  newWinApp->App_CreateWindow(width, height);

  _winApps.push_back(newWinApp);

  return newWinApp->GetHWND();
}
