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
// - Win32 API�� ����Ͽ� ���ø����̼� â�� ���� �� ����
// - Direct3D ��ġ���� ������ ���� HWND �ڵ��� ����
// - ����� �Է� �� �ý��� �޽��� ó���� ���� �޽��� ���� ����
// - â ũ�� �� ���� ����(��ü ȭ��, â ��� ��ȯ) ����