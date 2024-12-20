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
  _winApps.reserve(3); // ���� �ʱ�ȭ
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

bool WindowManager::DeleteWinApp()
{
  return false;
}

//LRESULT WindowManager::WndProc(HWND hWnd, UINT message, WPARAM wParam,
//                               LPARAM lParam)
//{
//  // â�� ���� Window ��ü�� �����ɴϴ�.
//  WinApp* pWinApp =
//      reinterpret_cast<WinApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
//
//  if (pWinApp)
//  {
//    // �ش� â�� �´� �޽��� ó��
//    return pWinApp->HandleMessage(hWnd, message, wParam, lParam);
//  }
//
//  // �⺻ ó��
//  return DefWindowProc(hWnd, message, wParam, lParam);
//}
