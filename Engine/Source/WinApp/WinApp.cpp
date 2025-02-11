#include "WinApp/WinApp.h"
#include <Windows.h>
#include "Core/Input/InputSystem.h"

#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

WinApp::WinApp(const wchar_t* className, HINSTANCE hInstance)
{
  _hInstance = hInstance;
  _className = className;

	// 생성자에서 MyRegisterClass() 를 해줘야 함
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = this->HandleMessage;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(107));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = className;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));

  if (!RegisterClassExW(&wcex))
  {
    MessageBox(nullptr, L"Class Registration Failed!", L"Error", MB_ICONERROR);
  }
}

void WinApp::App_CreateWindow(int width, int height)
{
  // 원하는 크기가 조정되어 리턴
  RECT rcClient = {0, 0, (LONG)width, (LONG)height};

  AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

  // 창 스타일 설정 플래그 (타이틀바 / 종료 / 최소화 / 최대화)
  DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION |
                  WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

// CreateWindowW() 함수 사용하여 hWnd 만들어줌.
  _hwnd = CreateWindowEx(0, _className, _className, dwStyle, CW_USEDEFAULT,
                         CW_USEDEFAULT, rcClient.right - rcClient.left,
                         rcClient.bottom - rcClient.top, nullptr,
                       nullptr,
                       _hInstance, nullptr);

// ShowWindow(_hWnd, SW_SHOWNORMAL); 사용하여 창 띄움
  ShowWindow(_hwnd, SW_SHOWNORMAL);
  UpdateWindow(_hwnd);
}

LRESULT CALLBACK WinApp::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                              LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    return true;

  switch (uMsg)
  {
#ifdef USED2D
  case WM_ACTIVATE:
    ShowCursor(FALSE);
    break;
#endif // USED2D

  case WM_CREATE:
  // TODO : 윈도우가 생성될 때 처리
    break;

  case WM_SIZE:
  // TODO: 윈도우 크기가 변경될 때 처리
    break;


  case WM_ACTIVATEAPP:
    // TODO: 애플리케이션이 활성화되거나 비활성화(else)될 때 처리
    DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
    DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
    break;

  case WM_POWERBROADCAST:
  // TODO: 시스템 전원 상태가 변경될 때 처리
  // 시스템이 대기 모드로 전환되려고 할 때 게임을 일시 중지합니다.
  // 시스템이 대기 모드에서 복귀할 때 게임을 재개합니다.
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_INPUT:
  case WM_MOUSEMOVE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  case WM_MOUSEWHEEL:
  case WM_XBUTTONDOWN:
  case WM_XBUTTONUP:
  case WM_MOUSEHOVER:
    DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
    break;

  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYUP:
    DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
    break;

  case WM_SYSKEYDOWN:
  // TODO: 시스템 키가 눌렸을 때 처리
  // ALT+ENTER 키 조합이 눌렸을 때 전체 화면 모드를 토글합니다.
    break;

  case WM_MENUCHAR:
  // TODO: 메뉴가 활성화된 상태에서 유효하지 않은 키를 눌렀을 때 처리
  // 오류 경고음을 방지하기 위해 메시지를 무시합니다.
  return MAKELRESULT(0, MNC_CLOSE);

  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}
