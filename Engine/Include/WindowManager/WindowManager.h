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
    - 윈도우 추가 및 제거 : Win32 API를 사용하여 애플리케이션 창을 생성 및 관리
    - 윈도우 속성 변경 : 창 크기 및 상태 변경(전체 화면, 창 모드 전환) 지원
    - Direct3D 장치와의 연결을 위해 HWND 핸들을 제공
    - 사용자 입력 및 시스템 메시지 처리를 위한 메시지 루프 관리
*/
