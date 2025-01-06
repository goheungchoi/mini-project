#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <stack>
#include <span>
#include <array>
#include <set>
#include <map>
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

constexpr uint64_t kUInt64Max{(std::numeric_limits<uint64_t>::max)()};

