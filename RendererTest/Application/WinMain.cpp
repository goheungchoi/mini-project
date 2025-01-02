// WinMain.cpp
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <../Application/GameApp/GameApp.h>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int WINAPI wWinMain(_In_ HINSTANCE hInstance, 
										_In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, 
										_In_ int nCmdShow)
{
  _CrtSetBreakAlloc(237);
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	GameApp app;
	app.Initialize();
	app.Execute();
	app.Shutdown();

	return 0;
}
