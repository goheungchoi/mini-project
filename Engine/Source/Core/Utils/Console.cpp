#include "Core/Utils/Console.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <tchar.h>
#include <windows.h>

// code from https://stackoverflow.com/a/57241985
bool AttachDebugConsole()
{
  if (!AllocConsole())
  {
    return false;
  }

  // std::cout, std::clog, std::cerr, std::cin
  FILE* fDummy;
  freopen_s(&fDummy, "CONOUT$", "w", stdout);
  freopen_s(&fDummy, "CONOUT$", "w", stderr);
  freopen_s(&fDummy, "CONIN$", "r", stdin);
  std::cout.clear();
  std::clog.clear();
  std::cerr.clear();
  std::cin.clear();

  // std::wcout, std::wclog, std::wcerr, std::wcin
  HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  // Set the std handles
  SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
  SetStdHandle(STD_ERROR_HANDLE, hConOut);
  SetStdHandle(STD_INPUT_HANDLE, hConIn);
  std::wcout.clear();
  std::wclog.clear();
  std::wcerr.clear();
  std::wcin.clear();

  // https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
  // Change the console mode to virtual terminal
  // Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
  {
    return false;
  }
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
  if (hIn == INVALID_HANDLE_VALUE)
  {
    return false;
  }

  DWORD dwOriginalOutMode = 0;
  DWORD dwOriginalInMode = 0;
  if (!GetConsoleMode(hOut, &dwOriginalOutMode))
  {
    return false;
  }
  if (!GetConsoleMode(hIn, &dwOriginalInMode))
  {
    return false;
  }

  DWORD dwRequestedOutModes =
      ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
  DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

  DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
  if (!SetConsoleMode(hOut, dwOutMode))
  {
    // We failed to set both modes, try to step down mode gracefully.
    dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    if (!SetConsoleMode(hOut, dwOutMode))
    {
      // Failed to set any VT mode, can't do anything here.
      return false;
    }
  }

  DWORD dwInMode = dwOriginalInMode | dwRequestedInModes;
  if (!SetConsoleMode(hIn, dwInMode))
  {
    // Failed to set VT input mode, can't do anything here.
    return false;
  }

  return true;
}

bool DetachDebugConsole()
{
  if (!FreeConsole())
    return false;
  return true;
}
