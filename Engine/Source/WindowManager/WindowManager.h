#pragma once
#include "../Core/Common.h"

class WindowManager
{
private:
  WindowManager() = delete;
  ~WindowManager() = delete;

public:
  //static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
  //                                LPARAM lParam);

  static HWND CreateWinApp(const wchar_t* className = L"WinClass",
                           HINSTANCE hInstance = GetModuleHandle(NULL));

};

// WindowManager
// - Win32 API를 사용하여 애플리케이션 창을 생성 및 관리
// - Direct3D 장치와의 연결을 위해 HWND 핸들을 제공
// - 사용자 입력 및 시스템 메시지 처리를 위한 메시지 루프 관리
// - 창 크기 및 상태 변경(전체 화면, 창 모드 전환) 지원