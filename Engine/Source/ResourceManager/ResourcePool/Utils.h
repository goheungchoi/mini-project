#pragma once

#include "Shared/Config/Config.h"

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include "Core/UUID/UUID.h"

inline fs::path GetResourcePath(xUUID uuid) {
	std::string strUUID = uuid.ToString();
	return fs::path(ns::kResourceDir) / strUUID.substr(0, 2) / strUUID;
}
