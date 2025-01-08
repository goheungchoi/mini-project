#pragma once
#include "Core/Common.h"

class WinApp;
class WindowManager
{

private:
    WindowManager();
    ~WindowManager();

    WindowManager(const WindowManager& wm) = delete;
    WindowManager& operator=(const WindowManager& wm) = delete;
    WindowManager(WindowManager&& wm) noexcept = delete;
    WindowManager& operator=(WindowManager&& wm) noexcept = delete;

  public:
    static WindowManager* GetInstance();

    HWND CreateWinApp(int width = 1920, int height = 1080,
                      const wchar_t* className = L"WinClass",
                           HINSTANCE hInstance = GetModuleHandle(NULL));
  bool DeleteWinApp();

  bool DestroyWindowManager();
  //LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
  //                         LPARAM lParam);

private:
  static WindowManager* m_pInstance;
  std::vector<WinApp*> _winApps;

};

/*
WindowManager
    - ������ �߰� �� ���� : Win32 API�� ����Ͽ� ���ø����̼� â�� ���� �� ����
    - ������ �Ӽ� ���� : â ũ�� �� ���� ����(��ü ȭ��, â ��� ��ȯ) ����
    - Direct3D ��ġ���� ������ ���� HWND �ڵ��� ����
    - ����� �Է� �� �ý��� �޽��� ó���� ���� �޽��� ���� ����
*/
