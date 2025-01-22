#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

//use comptr
#include <wrl.h>

#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <optional>
#include <string>
using namespace std::literals;
#include <string_view>
#include <vector>
#include <queue>
#include <stack>
#include <span>
#include <array>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <functional>
#include <type_traits>
#include <algorithm>
#include <concepts>
#include <any>
#include <ranges>
#include <filesystem>
namespace fs = std::filesystem;

// #include <fmt/core.h>
// #include <fmt/format.h>

namespace Utility
{
#if _WIN32
inline std::string convertToUTF8(const std::wstring& wstr)
{
  const int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1,
                                             nullptr, 0, nullptr, nullptr);
  const std::unique_ptr<char[]> buffer(new char[bufferSize]);
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer.get(), bufferSize,
                      nullptr, nullptr);
  return std::string(buffer.get());
}
inline std::wstring convertToUTF16(const std::string& str)
{
  const int bufferSize =
      MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
  const std::unique_ptr<wchar_t[]> buffer(new wchar_t[bufferSize]);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.get(), bufferSize);
  return std::wstring(buffer.get());
}
#endif // _WIN32
};


constexpr uint64_t kUInt64Max{(std::numeric_limits<uint64_t>::max)()};

