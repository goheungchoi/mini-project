#include "WinApp.h"

WinApp::WinApp(const wchar_t* className, HINSTANCE hInstance)
{
  _hInstance = hInstance;

	// �����ڿ��� MyRegisterClass() �� ����� ��
}

void WinApp::App_CreateWindow(int width, int height)
{
// CreateWindowW() �Լ� ����Ͽ� hWnd �������.
  //_hwnd = CreateWindowEx();

// ShowWindow(_hWnd, SW_SHOWNORMAL); ����Ͽ� â ���
 // ShowWindow(_hwnd, SW_SHOWNORMAL);
}
