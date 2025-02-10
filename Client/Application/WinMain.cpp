// Cient::WinMain.cpp
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include "GameApp/GameApp.h"
#include <Windows.h>

#ifdef _DEBUG
#include "Core/Utils/Console.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <Dbghelp.h>
#include <tchar.h>

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(
    HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

void create_minidump(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
  HMODULE mhLib = ::LoadLibrary(_T("dbghelp.dll"));
  MINIDUMPWRITEDUMP pDump =
      (MINIDUMPWRITEDUMP)::GetProcAddress(mhLib, "MiniDumpWriteDump");

  HANDLE hFile = ::CreateFile(_T("minidump.dmp"), GENERIC_WRITE, FILE_SHARE_WRITE,
                              NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  _MINIDUMP_EXCEPTION_INFORMATION ExInfo;
  ExInfo.ThreadId = ::GetCurrentThreadId();
  ExInfo.ExceptionPointers = apExceptionInfo;
  ExInfo.ClientPointers = FALSE;

  pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal,
        &ExInfo, NULL, NULL);
  ::CloseHandle(hFile);
}

LONG WINAPI unhandled_exception_handler(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
  create_minidump(apExceptionInfo);
  return EXCEPTION_CONTINUE_SEARCH;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
  //_CrtSetBreakAlloc(568);
#ifdef _DEBUG
  AttachDebugConsole();
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // Write a dump file when an unhandled exception occurred.
  SetUnhandledExceptionFilter(unhandled_exception_handler);

  GameApp app;
  app.Initialize(kScreenWidth, kScreenHeight, L"Rebellion");
  app.Execute();
  app.Shutdown();

#ifdef _DEBUG
	DetachDebugConsole();
#endif

  return 0;
}
