#include "WindowManager/WindowManager.h"
#include "WinApp/WinApp.h"

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
  if (!_winApps.empty())
  {
    for (auto& winapp : _winApps)
    {
      delete winapp;
      winapp = nullptr;
    }
  }

  return true;
}

bool WindowManager::DestroyWindowManager()
{
  if (m_pInstance)
  {
    delete m_pInstance;
    m_pInstance = nullptr;
  }

  return true;
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
