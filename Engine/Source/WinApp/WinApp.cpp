#include "WinApp.h"
#include <Windows.h>


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
// CreateWindowW() 함수 사용하여 hWnd 만들어줌.
  _hwnd = CreateWindowEx(0, _className, _className,
                         WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                       CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr,
                       nullptr,
                       _hInstance, nullptr);

// ShowWindow(_hWnd, SW_SHOWNORMAL); 사용하여 창 띄움
  ShowWindow(_hwnd, SW_SHOWNORMAL);
  UpdateWindow(_hwnd);
}

LRESULT CALLBACK WinApp::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                              LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}
