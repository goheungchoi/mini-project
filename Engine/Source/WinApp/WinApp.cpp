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

	// �����ڿ��� MyRegisterClass() �� ����� ��
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
  // â ��Ÿ�� ���� �÷��� (Ÿ��Ʋ�� / ���� / �ּ�ȭ / �ִ�ȭ)
  DWORD dwStyle =
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

// CreateWindowW() �Լ� ����Ͽ� hWnd �������.
  _hwnd = CreateWindowEx(0, _className, _className, dwStyle,
                       CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr,
                       nullptr,
                       _hInstance, nullptr);

// ShowWindow(_hWnd, SW_SHOWNORMAL); ����Ͽ� â ���
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
  case WM_CREATE:
  // TODO : �����찡 ������ �� ó��
    break;

  case WM_SIZE:
  // TODO: ������ ũ�Ⱑ ����� �� ó��
    break;


  case WM_ACTIVATEAPP:
    // TODO: ���ø����̼��� Ȱ��ȭ�ǰų� ��Ȱ��ȭ(else)�� �� ó��
    DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
    DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
    break;

  case WM_POWERBROADCAST:
  // TODO: �ý��� ���� ���°� ����� �� ó��
  // �ý����� ��� ���� ��ȯ�Ƿ��� �� �� ������ �Ͻ� �����մϴ�.
  // �ý����� ��� ��忡�� ������ �� ������ �簳�մϴ�.
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
  // TODO: �ý��� Ű�� ������ �� ó��
  // ALT+ENTER Ű ������ ������ �� ��ü ȭ�� ��带 ����մϴ�.
    break;

  case WM_MENUCHAR:
  // TODO: �޴��� Ȱ��ȭ�� ���¿��� ��ȿ���� ���� Ű�� ������ �� ó��
  // ���� ������� �����ϱ� ���� �޽����� �����մϴ�.
  return MAKELRESULT(0, MNC_CLOSE);

  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}
