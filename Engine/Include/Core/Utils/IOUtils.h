#pragma once

#include "Core/Common.h"

[[nodiscard]]
std::vector<char> ReadFile(const fs::path& filepath);
