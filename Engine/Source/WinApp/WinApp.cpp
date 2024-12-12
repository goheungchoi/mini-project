#include "WinApp.h"

WinApp::WinApp(const wchar_t* className, HINSTANCE hInstance)
{
  _hInstance = hInstance;

	// 생성자에서 MyRegisterClass() 를 해줘야 함
}

void WinApp::App_CreateWindow(int width, int height)
{
// CreateWindowW() 함수 사용하여 hWnd 만들어줌.
  //_hwnd = CreateWindowEx();

// ShowWindow(_hWnd, SW_SHOWNORMAL); 사용하여 창 띄움
 // ShowWindow(_hwnd, SW_SHOWNORMAL);
}
