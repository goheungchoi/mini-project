// WinMain.cpp
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, 
										_In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, 
										_In_ int nCmdShow) {
  while (1) {
  }

  return 0;
}
